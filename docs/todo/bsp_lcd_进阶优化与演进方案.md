# bsp_lcd 液晶驱动进阶优化与演进方案

本文件详细记录了 `bsp_lcd` 液晶驱动在验证通过后实施的进阶优化点，并综合规划了未来面向 LVGL 图形库、高性能显示以及驱动解耦的后续演进方案。

---

## 1. 已落地的进阶优化成果

在 M12 阶段首期实现 LCD 全屏颜色填充后，我们针对长期运行的系统鲁棒性与 CPU 开销完成了以下两项关键优化：

### 1.1 静态 DMA 缓冲区设计（规避堆碎片化风险）
* **背景与风险**：
  在早期的清屏逻辑中，每次调用 `bsp_lcd_clear` 均会动态向堆内存申请一段 `MALLOC_CAP_DMA` 兼容的缓冲区，用毕后释放。这在系统高频刷新时会导致片内 DMA SRAM 产生碎片化 physical hole，长期运行可能导致系统因 OOM（无法分配连续内存）而死机。
* **优化策略**：
  在 `bsp_lcd_init` 初始化阶段，一次性静态申请常驻缓冲：
  ```c
  s_clear_buffer = heap_caps_malloc(LCD_H_RES * CLEAR_BUFFER_LINES * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  ```
  在生命周期内永久复用此缓冲区，彻底规避了系统碎片化崩溃风险。

### 1.2 硬件级字节序大小端转换（零 CPU 字节交换）
* **背景与性能损耗**：
  ESP32-S3 原生采用小端（Little-Endian）数据存储，而 LCD 控制器常规要求大端（Big-Endian）流。原先需要软件循环将 16-bit 像素逐个进行位移拼接：
  ```c
  uint16_t swapped_color = (color >> 8) | (color << 8); // 增加了 CPU 指令
  ```
* **优化策略**：
  在初始化配置中，将 `.data_endian` 设置为 `LCD_RGB_DATA_ENDIAN_LITTLE`。驱动会自动配置 ST7789 芯片的 `RAMCTRL`（0xB0 寄存器），使其接收数据后在屏幕硬件层自动倒序重组。
* **效果**：
  `bsp_lcd_clear` 等绘图函数可直接把 CPU 原生小端数组传给 DMA：
  ```c
  s_clear_buffer[i] = color; // 零 CPU 开销
  ```

---

## 2. 后续演进方案与路线图 (Roadmap)

为了进一步提升整机交互的流畅度，我们将屏幕驱动的后续优化归纳为以下几项待演进模块：

### 2.1 M14 阶段：面向 LVGL 的“异步中断传输”优化 [规划中 📅]
* **拟改进场景**：
  当前在清屏或绘图时，调用 `esp_lcd_panel_draw_bitmap` 后，如果总线队列满，CPU 必须等待 DMA 数据全部在 SPI 线上发送完成才能继续，这是一种半同步的“阻塞等待”。
* **具体设计**：
  1. 在 M14 阶段集成 LVGL 时，在 `esp_lcd_new_panel_io_spi` 的配置中注册 `.on_color_trans_done` 硬件中断回调函数。
  2. 当 LVGL 发送刷屏区块数据时，调用异步的 `esp_lcd_panel_draw_bitmap` 后 CPU 立刻返回执行 LVGL 业务线程或传感器读取。
  3. 当 SPI 控制器硬件完成数据传输后，会自动触发完成中断。在中断 ISR 中向 LVGL 驱动层发送 `lv_disp_flush_ready()` 信号，通知其可以刷新下一区块。
* **目的**：通过并发执行“CPU 渲染 UI <--> SPI 硬件传输”，实现 CPU 零忙等。

### 2.2 SRAM 极速乒乓双缓冲 (Ping-Pong Buffering) [规划中 📅]
* **拟改进场景**：
  由于开发板搭载了 8MB 八线大容量 PSRAM，部分开发者倾向于在 PSRAM 中分配全屏双缓冲。然而 PSRAM 带宽和延迟均不如片内 SRAM。
* **具体设计**：
  配合 2.1 节的异步中断，在内部快速 SRAM 中开辟 **两块尺寸适中（如屏幕高度的 1/10，即 24 行像素，共约 30.7KB）的双缓冲区**。
* **工作机制**：
  * 当 DMA 从 Buffer A 中搬运数据发送至 ST7789 时，CPU 完全并行地将下一帧的 UI 渲染写入 Buffer B。
  * 发送完成后在中断中瞬间交换指针，DMA 开始从 Buffer B 搬运，CPU 转向渲染 Buffer A。
* **目的**：利用 SRAM 的无缓存抖动（Zero Cache Miss）极速读写特性，在低物理内存消耗下，帧率显著优于 PSRAM 大缓冲方案。

### 2.3 SPI 时序校正与 Dummy 周期自适应调节 [技术预研中 🔬]
* **拟改进场景**：
  目前屏幕工作在 80MHz 极限主频，且走线经过了 GPIO Matrix 路由。如果量产或在复杂温度下运行，可能有出现细微信号畸变的隐患。
* **具体设计**：
  1. 开启 `CONFIG_SPI_MASTER_ISR_IN_IRAM` 配置，将 SPI 中断向量放入片内 IRAM 降低中断延迟。
  2. 针对 GPIO 矩阵延迟，根据需要在 Panel IO 配置中开启 dummy cycles 补偿，以抵消引脚读写反馈滞后。
  3. 如有稳定性需求，可动态下调至 `40MHz + Mode 0` 进行基准稳定性测试。

### 2.4 板级硬件参数参数化（解耦优化） [待重构 📅]
* **拟改进场景**：
  目前 `bsp_lcd.c` 中硬编码了引脚号（39、40、41）及分辨率。
* **具体设计**：
  重构 `bsp_lcd_init` API 接口，引入配置结构体：
  ```c
  typedef struct {
      int sclk_io_num;
      int mosi_io_num;
      int dc_io_num;
      uint32_t pclk_hz;
      int spi_mode;
  } bsp_lcd_config_t;
  
  esp_err_t bsp_lcd_init_with_config(const bsp_lcd_config_t *config);
  ```
  进一步将液晶底层组件解耦为通用驱动。
