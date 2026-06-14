# bsp_lvgl_port 移植层进阶性能优化方案

根据开发板核心硬件（ESP32-S3 双核, 16MB Octal Flash, 8MB Octal PSRAM, SPI 80MHz 液晶接口）的特点，我们在 M14 成功完成 LVGL 9.x 稳定移植的基础上，提取出以下针对本项目的进阶性能优化与算力榨取方案。

---

## 1. 内存策略博弈：片内 SRAM 乒乓缓冲 vs 外部 PSRAM 大渲染块

### 1.1 方案对比与适用场景
目前我们在 `bsp_lvgl_port.c` 中开启了 24 行像素（$1/10$ 屏幕高度）的内部 SRAM 双缓冲，这是零 Cache Miss、低延迟的经典配置。但随着 UI 复杂度的提升，有两种策略可供演进：

*   **策略 A：SRAM 乒乓缓冲（当前使用）**
    *   *缓冲大小*：320 * 24 * 2 = 15.3KB (SRAM)
    *   *特点*：访问无延迟，CPU 写 Buffer 速度极快。
    *   *适用*：偏向静态数据展示、简单局部动效的 UI。
*   **策略 B：PSRAM 大渲染块（进阶候选）**
    *   *配置方式*：设置 `buff_spiram = true`，并将 Buffer 深度扩大至 80 ~ 120 行像素（$1/3 \sim 1/2$ 屏）。
    *   *原理*：虽然 PSRAM 存取速度较 SRAM 慢，但将单帧图像分割从 10 次渲染/传输缩减到 2~3 次。能显著减少 FreeRTOS 上下文切换、DMA 握手以及中断服务的响应开销。
    *   *适用*：未来若接入全屏滑动、大面积透明度混合（Alpha Blending）或大图缩放等重度计算 UI。

---

## 2. 多核算力隔离（Core Affinity）

在单核上运行 Wi-Fi 协议栈与图形渲染，当网络高频收发包或进行配网通信时，极易抢占 CPU 时间片导致 UI 渲染任务被挂起，产生肉眼可见的卡顿。

### 优化实现：
*   **物理隔离策略**：
    1.  强制将 LVGL 运行任务（`lv_timer_handler` 线程）绑定到 **Core 1** 上（可通过 `lvgl_port_cfg_t` 的 `task_affinity` 参数配置为 `1`，或在 `menuconfig` 中显式指定）。
    2.  将底层的 Wi-Fi/NimBLE 无线协议栈、LwIP、LittleFS I/O 读写以及其它后台传感器驱动留在 **Core 0** 运行。
*   **目的**：确保网络配网的高流量包处理不干扰 UI 帧率，提供物理级别的丝滑人机交互体验。

---

## 3. 总线带宽榨取与物理天花板

### 优化实现：
*   **总线满血配置**：
    *   在 `menuconfig` 的 `Serial flasher config` 与 `Spiram config` 中，确保 16MB Flash 和 8MB PSRAM 配置为最高性能（OPI 模式，八线传输），并将工作频率拉满至 **80MHz**，加快 Cache 命中与填充速度。
*   **SPI 中断加速**：
    *   在 sdkconfig 中开启 `CONFIG_SPI_MASTER_ISR_IN_IRAM=y`，将 SPI 中断服务函数（ISR）强制加载在内部 IRAM，降低每次 DMA 发送完成后的响应延时。

---

## 4. 编译器优化级别调整

### 优化实现：
*   在 `menuconfig` 的 `Compiler options -> Optimization Level` 选项中，将默认的 `Optimize for size (-Os)` 切换为 **Optimize for performance (-O2)**。
*   **目的**：全面释放 GCC 编译器对 LVGL 图形混合、抗锯齿像素运算法则的循环展开与指令集优化，压榨出最高的计算效率。
