# M14 阶段：LVGL 9.x 移植旋转参数与大小端未对齐导致的花屏花色异常修复记录

## 1. 问题现场描述

在完成组件化移植、基于 `espressif/esp_lvgl_port` 组件替换手写初始化适配层后，系统在首次加载显示时发生严重渲染异常：
1. **屏幕显示偏移与黑边**：液晶面板左半边呈现正方形框状图像，而右侧大约 $1/4$ 面积呈黑色。
2. **条纹花屏**：白色框线内部充满了横向/纵向交错的蓝色和白色条纹。
3. **Demo 异常**：控制台调用 `ui_demo widgets` 后，全屏充满规律性的条纹色块，无法识别任何 UI 组件及文本。

---

## 2. 软硬件运行环境

*   **SoC 控制芯片**：ESP32-S3-WROOM-1 (N16R8)
*   **LCD 屏幕控制器**：ST7789V（物理显示分辨率 $320 \times 240$，横屏接口）
*   **通信接口**：SPI 极速主从串行接口（工作频率 80MHz，SCLK/MOSI 开启 GPIO 矩阵对齐）
*   **系统软件环境**：ESP-IDF v5.5.1
*   **图形框架版本**：LVGL v9.3.0 (`espressif/esp_lvgl_port` 官方移植组件)

---

## 3. 原因排查与定位

### 现象一的起因：坐标轴配置重置（导致 $1/4$ 黑边与条纹花屏）
1. ST7789 图形芯片的内部物理显存默认是以纵向 $240 \times 320$ （竖屏模式）映射排线像素的。
2. 为了支持项目所需的横屏（$320 \times 240$）物理方向，我们在 BSP 底层驱动 `bsp_lcd.c` 中显式设置了：
   ```c
   esp_lcd_panel_swap_xy(s_panel_handle, true);        // 开启 XY 坐标对调
   esp_lcd_panel_mirror(s_panel_handle, true, false);  // 配置 X 轴镜像映射
   ```
3. 然而，官方组件 `esp_lvgl_port` 在执行 `lvgl_port_add_disp()` 时，会获取配置结构体 `lvgl_port_display_cfg_t` 中的 `.rotation` 数据，并通过 LCD 底层接口动态覆盖之前在 `bsp_lcd.c` 中的坐标轴设定。
4. 我们原先在 `bsp_lvgl_port.c` 初始化时，没有给 `disp_cfg` 的 `rotation` 域赋值（默认全为 `false`）。
5. 组件启动后，其默认的 `rotation` 逻辑强行将控制器重置为：
   ```c
   esp_lcd_panel_swap_xy(control_handle, false);
   esp_lcd_panel_mirror(control_handle, false, false);
   ```
6. **后果**：
   *   屏幕被强制扭转回了物理竖屏结构（显存列数仅有 240 列，行数 320 行）。
   *   而 LVGL 发送像素数据时，依然按逻辑横屏（每行 320 像素）向显存寻址。
   *   最右侧的 $320 - 240 = 80$ 像素（恰好占物理宽度的 $\frac{1}{4}$）由于写超了列范围限制，完全无法被正常渲染，保留为初始复位状态的**黑色**。
   *   因为每一行本应是 320 个像素的数据，被强行按 240 个一截断排列，导致内存数据发生了空间错位（维度转置），所有矩形、文字的纯色块在屏幕上全部拉伸畸变，变为了规则的**条纹色块**。

### 现象二的起因：大小端匹配逻辑（潜在的花色/反色隐患）
1. 液晶控制器 ST7789 在 SPI 总线上默认接收高字节在前、低字节在后的 Big-Endian 格式数据。
2. 我们在 `bsp_lcd.c` 初始化 panel 配置时，特地加入了对硬件原生小端数据解析的支持：
   ```c
   .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE
   ```
   这使得 ST7789 控制器接收到小端数据后会在内部自动重组，避免了 CPU 每次清屏或绘图时进行字节交换（`Byte-Swap`）的软件开销。
3. 相应的，LVGL 的原生像素格式即为 Little-Endian。由于两者在硬件层已完成了小端到小端的握手，因此在 `esp_lvgl_port` 配置中，应当确保 `.flags.swap_bytes = false`。若再次开启此项，反而会导致像素发生二次错位反色。

---

## 4. 解决措施与代码修正

针对此问题，需显式声明 `esp_lvgl_port` 注册显示设备时的旋转状态和字节序。

### 修复代码

修改 `bsp/lvgl_port/bsp_lvgl_port.c` 中第 47 - 65 行处的配置项结构体定义：

```diff
     const lvgl_port_display_cfg_t disp_cfg = {
         .io_handle = io,
         .panel_handle = panel,
         .buffer_size = LCD_H_RES * 24, // 设置缓冲区深度为 24 行像素 (1/10 屏幕高度)
         .double_buffer = true,        // 启用乒乓双缓冲提高并发帧率
         .hres = LCD_H_RES,
         .vres = LCD_V_RES,
+        .color_format = LV_COLOR_FORMAT_RGB565,
+        .rotation = {
+            .swap_xy = true,          // 与 bsp_lcd 硬件初始化时的配置严格对齐，恢复横屏坐标轴
+            .mirror_x = true,         // 配置 X 方向镜像映射排线
+            .mirror_y = false,
+        },
         .flags = {
             .buff_dma = true,         // 使用 DMA 缓冲
             .buff_spiram = false,     // 禁用外部 PSRAM，强制存放在内部 SRAM (Zero Cache Miss)
+            .swap_bytes = false,      // 因为 ST7789 已配置硬件小端解析 (LCD_RGB_DATA_ENDIAN_LITTLE)，故 CPU 侧无需字节交换
         }
     };
```

---

## 5. 验证与验收

1. **静态验证**：
   清理并编译工程：
   ```bash
   idf.py fullclean
   idf.py build
   ```
   没有产生任何新增编译警告，顺利生成 bin 固件。
2. **动态功能验证**：
   *   烧录并运行后，默认加载的 UI 画面正确显示，边缘无花边、无任何画面倾斜或拉伸畸变；
   *   输入 `ui_demo widgets` 指令，全屏渲染出官方经典控件，画面正常且没有 $1/4$ 黑色空白；
   *   输入 `ui_demo music`，能够流畅渲染音乐播放组件，坐标完美匹配电容触摸传感器，通过点按可以正常进行组件互动；
   *   色彩呈现完全正确，未出现反色、杂色斑块。
