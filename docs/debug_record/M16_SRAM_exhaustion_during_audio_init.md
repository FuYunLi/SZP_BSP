# M16 音频初始化触发内部 SRAM 耗尽与卡死调试记录

## 1. 问题背景与异常现象

在集成 M16 音频总线与功放使能控制后，系统在上电启动阶段偶发性地出现看门狗超时复位及 LCD 屏幕刷新超时挂起。具体异常日志如下：

```text
E (2210) lcd_panel.io.spi: panel_io_spi_tx_color(395): spi transmit (queue) color failed
E (2220) lcd_panel.st7789: panel_st7789_draw_bitmap(227): io tx color failed
...
E (17144) task_wdt: Task watchdog got triggered. The following tasks/users did not reset the watchdog in time:
E (17144) task_wdt:  - IDLE1 (CPU 1)
E (17144) task_wdt: Tasks currently running:
E (17144) task_wdt: CPU 0: IDLE0
E (17144) task_wdt: CPU 1: taskLVGL
```

同时，在控制台中手动触发 `audio_init` 诊断指令时，会出现以下错误：
```text
I (42765) bsp_audio: 正在初始化 I2S0 标准通道 (TX/RX)...
I (42775) bsp_audio: I2S TX 通道初始化 STD 模式成功
E (42775) i2s_common: i2s_alloc_dma_desc(510): allocate DMA buffer failed
E (42785) i2s_std: i2s_std_set_slot(122): allocate memory for dma descriptor failed
E (42805) bsp_audio: 初始化 I2S RX 通道失败: ESP_ERR_NO_MEM
```

---

## 2. 调试与排查过程

### 2.1 引脚与外设冲突排查
首先比对了 I2S0 时钟/数据引脚与 LCD SPI 的物理连线：
* **I2S0**: MCK->IO38, BCK->IO14, WS->IO13, DO->IO45, DI->IO12
* **SPI2 (LCD)**: CLK->IO41, MOSI->IO40, DC->IO39, CS->PCA9557_IO0
经核对原理图与源码，两外设的 GPIO 矩阵配置及 DMA 控制器完全独立，不存在直接的硬件信号冲突。

### 2.2 核心根源定位：内部 SRAM 耗尽
* I2S 驱动的 DMA 描述符（DMA Descriptors）及对应的传输缓冲区（Data Buffers）根据 ESP-IDF 的规范，**必须分配在内部 SRAM（MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL） 中**。
* 当前系统在启动时集成了 **Wi-Fi 协议栈**、**NimBLE 蓝牙 Host/Controller** 以及 **LVGL 缓存（主屏刷新）**，这些组件极度贪婪，在默认配置下会抢占绝大部分的内部 SRAM。
* 当系统按照默认参数初始化 I2S0 TX 和 RX 双通道时，SRAM 彻底告急，导致 RX 通道申请 DMA 描述符失败（抛出 `ESP_ERR_NO_MEM`）。
* 此后，当 `taskLVGL` 启动并尝试刷新首帧画面时，SPI 驱动由于内部 SRAM 不足而无法为新的事务数据排队，抛出 `panel_io_spi_tx_color failed`。LVGL 任务因此无限期挂起在 `wait_for_flushing` 等待机制中，导致 IDLE 任务无法被调度，从而触发了 CPU1 上的任务看门狗（WDT）复位。

---

## 3. 改进方案

为保障系统的长期稳定性，我们采取了 **代码端轻量化** 与 **系统端内存池优化** 双管齐下的改进方案：

### 3.1 代码端驱动优化 (bsp_audio.c)
* **按需分配，裁剪 RX**：在 M16（音频播放与功放使能）阶段，我们仅对播放（I2S0 TX）进行初始化，暂时不为录音（I2S0 RX）分配 DMA 通道（`rx_handle` 传 `NULL`），直接省去了一半的 I2S DMA SRAM 开销。
* **精简 DMA 配置参数**：将 TX 通道的 DMA 参数进行裁剪，`dma_desc_num` 从默认的 6 降为 4，`dma_frame_num` 从默认的 240 降为 128，极大地缩减了 I2S 数据缓冲区对内部 SRAM 的强占。

### 3.2 系统端配置优化 (sdkconfig.defaults)
通过将占用大量临时/全局缓存的网络协议栈内存移入外部的 8MB PSRAM 中，为内部 DMA 腾出充裕的空间：
```ini
# 音频与系统内存优化：将 Wi-Fi 和 NimBLE 蓝牙内存池路由至外部 PSRAM
CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL=n
CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL=y
CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP=y
CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=4096
```
* **Wi-Fi 与 LwIP 优化**：启用 `CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP` 将网络接收/发送缓存放到 PSRAM 中。
* **蓝牙优化**：启用 `CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL` 将蓝牙 Host 内存池彻底放到 PSRAM 中。
* **分配阈值优化**：将 `CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL` 阈值调低至 4KB，使得大于 4KB 的非 DMA 动态申请都走外部内存。

---

## 4. 当前现状

在控制台进行测试，诊断命令全部运行正常：
* 执行 `audio_pa 1`：PCA9557 顺利将功放拉高，PA 处于正常供电。
* 执行 `audio_init`：I2S0 TX 通道在优化后的 DMA 参数下顺利分配，不再抛出 `ESP_ERR_NO_MEM`。
* 屏幕显示、触摸交互、IMU 读取等功能在 Wi-Fi 和蓝牙共存下均稳定，再未出现 SPI 发送失败及 WDT 挂死。
