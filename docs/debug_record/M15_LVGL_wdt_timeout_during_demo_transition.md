# M15 阶段：LVGL 界面切换触发 Task Watchdog Timeout (TWDT) 与多线程冲突异常修复记录

## 1. 问题现场描述

在 M15 阶段引入并集成了 `wifi_service`（基于事件驱动与 NVS 存储的 Wi-Fi 服务）和 `ble_service`（基于 NimBLE 的蓝牙服务）后，系统底层网络模块工作正常。然而，在控制台命令行终端输入 `ui_demo widgets`、`ui_demo music` 或其他 UI Demo 切换指令时，系统会立即卡死并触发 **Task Watchdog Timeout (TWDT)**，导致整机复位重启。

### 异常现场日志

串口输出的看门狗超时日志及调用栈如下：

```text
E (99920) task_wdt: Task watchdog got triggered. The following tasks/users did not reset the watchdog in time:
E (99920) task_wdt:  - IDLE0 (CPU 0)
E (99920) task_wdt: Tasks currently running:
E (99920) task_wdt: CPU 0: console_repl
E (99920) task_wdt: CPU 1: IDLE1
E (99920) task_wdt: Print CPU 0 (current core) backtrace

Backtrace: 0x42091466:0x3FCA98F0 0x420917D4:0x3FCA9910 0x4037C936:0x3FCA9940 0x4201D696:0x3FCD5090 0x4200E531:0x3FCD50B0 0x4200F996:0x3FCD50D0 0x4208AD97:0x3FCD50F0 0x4208B02A:0x3FCD5110 0x403877A9:0x3FCD5140
--- 0x42091466: task_wdt_timeout_handling at /home/liyun/esp/v5.5.1/esp-idf/components/esp_system/task_wdt/task_wdt.c:436
--- 0x420917d4: task_wdt_isr at /home/liyun/esp/v5.5.1/esp-idf/components/esp_system/task_wdt/task_wdt.c:509  
--- 0x4037c936: _xt_lowint1 at /home/liyun/esp/v5.5.1/esp-idf/components/xtensa/xtensa_vectors.S:1240
--- 0x4201d696: lv_obj_clean at /home/liyun/esp/projects/LCSZP/SZP_BSP/V1/managed_components/lvgl__lvgl/src/core/lv_obj_tree.c:95
--- 0x4200e531: app_ui_show_demo at /home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/app_ui.c:24
--- 0x4200f996: do_ui_demo_cmd at /home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/app_cli.c:778
```

---

## 2. 软硬件运行环境

*   **SoC 控制芯片**：ESP32-S3-WROOM-1 (N16R8)，双核芯片 (Core 0 / Core 1)
*   **LCD & Touch 硬件**：ST7789V 屏幕，GT911 电容触摸屏
*   **系统软件环境**：ESP-IDF v5.5.1 (FreeRTOS SMP 调度)
*   **图形框架版本**：LVGL v9.3.0 (`espressif/esp_lvgl_port` 官方移植组件)
*   **网络协议栈与后台服务**：Wi-Fi (STA/AP 模式)、NimBLE (蓝牙主机协议栈)

---

## 3. 原因排查与定位

根据看门狗超时机理、多线程交互流程以及调用栈分析，问题由以下几个维度的原因重叠导致：

### 3.1 核心空闲任务（IDLE0）被长时间饿死
1. ESP-IDF 在双核处理器上运行 FreeRTOS SMP，其中 `IDLE0` 运行在 Core 0，`IDLE1` 运行在 Core 1。
2. 控制台服务命令行任务 `console_repl` 运行在 **Core 0**（优先级为 2）。
3. 当执行 `ui_demo` 命令时，`console_repl` 任务同步获取 LVGL 互斥锁，并在自身任务上下文中执行 `app_ui_show_demo` -> `lv_obj_clean` 以及后续复杂的 UI 对象销毁与重建动作。
4. 同步销毁庞大的 Widgets / Music Demo 的 UI 对象树极其耗时（频繁遍历、内存释放、重置输入设备状态），导致 `console_repl` 任务在 Core 0 上持续占满 100% CPU 算力而不让出调度。
5. 优先级更低的 `IDLE0` 任务（优先级 0）因此被长时间饿死，无法在 5 秒的超时期内运行喂狗，从而触发 CPU 0 上的 TWDT 超时硬复位。

### 3.2 跨线程同步 UI 操作引发多线程竞态与闭环死循环
1. 虽然使用了 `lvgl_port_lock(0)` 进行锁保护，但是在非 UI 主线程（控制台 `console_repl` 任务）的上下文中**同步**操作 UI 树结构依然蕴含巨大隐患：
   * `lv_obj_clean` 会递归调用 `obj_delete_core`。在删除子对象时，如果这些对象绑定了定时器、动画或输入设备（指针设备 `pointer` 处于 `PRESSED` 状态），删除操作会试图将输入设备重置。
   * 如果此时运行在 CPU 1（或正在调度）的 `taskLVGL` 与 CPU 0 的控制台任务产生事件交互，可能会破坏 LVGL 内部树状列表的节点关系。
2. **后果**：链表内部逻辑受损，使 `lv_obj_tree.c:95` 中的 `while(child)` 陷入无法跳出的死循环，进而使 CPU 0 陷入死锁式的 100% 独占，必然触发看门狗。

### 3.3 核心未绑定导致资源激烈抢占
1. 默认情况下，官方 `esp_lvgl_port` 组件创建的 UI 主线程 `taskLVGL` 的 `task_affinity` 参数为 `-1`。这使得 UI 渲染主任务在 Core 0 和 Core 1 之间随机调度。
2. 同时，Wi-Fi 协议栈、蓝牙协议栈（NimBLE）、LwIP 任务以及 Flash 读写均强行绑定在 Core 0。
3. 当 `taskLVGL` 飘到 Core 0 运行时，会与无线协议栈产生频繁的时间片争抢，加剧了任务调度延迟与 CPU 0 的负荷，极易在执行重型 UI 时产生卡顿或死锁崩溃。

---

## 4. 解决措施与方案设计

为彻底消除 TWDT 看门狗风险，需执行**“多核物理隔离”**与**“异步 UI 派发调度”**的联合整改方案。

### 4.1 方案一：引入 `lv_async_call` 实现异步 UI 派发
不要在 CLI 或其他后台服务线程的上下文中直接执行复杂的 LVGL API（如清屏、销毁和重新创建对象）。
1. 控制台命令处理函数在锁保护下，仅将 UI 切换任务的名称复制到静态缓冲区，并注册一个异步回调：
   ```c
   lv_async_call(async_show_demo_cb, s_next_demo_name);
   ```
2. 随后立即释放互斥锁并返回。整个过程耗时仅微秒级，CLI 任务快速释放 CPU 并进入挂起等待 UART 状态，绝不饿死 `IDLE0`。
3. LVGL 自身的轮询主任务 `taskLVGL` 会在下一个 tick 周期中，在自身上下文中安全地执行 `async_show_demo_cb` 里的 UI 切换逻辑，完全避免了跨线程生命周期竞态问题。

### 4.2 方案二：实施多核绑定（Core Affinity）隔离
修改 `bsp/lvgl_port/bsp_lvgl_port.c`，把 UI 主渲染任务强制绑定在 **Core 1** 运行，实现计算力隔离：
* **Core 0**：专门负责运行 Wi-Fi 协议栈、NimBLE 蓝牙协议栈、LwIP、文件系统 I/O 和控制台 CLI。
* **Core 1**：专门负责运行 UI 渲染任务 `taskLVGL` 以及读取触摸屏。

---

## 5. 代码修复实践

### 5.1 修改 `bsp/lvgl_port/bsp_lvgl_port.c` (多核绑定)

```diff
@@ -24,8 +24,9 @@ esp_err_t bsp_lvgl_port_init(void)
     ESP_LOGI(TAG, "正在初始化乐鑫官方 esp_lvgl_port 框架...");
 
     // 1. 初始化 LVGL 接口框架与任务机制（内部使用高精度定时器并创建运行 lv_timer_handler 的 FreeRTOS 任务）
-    const lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
+    lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
+    port_cfg.task_affinity = 1; // 显式绑定至 Core 1 (CPU 1)，从而与运行于 Core 0 的协议栈及 console_repl 物理隔离
     esp_err_t err = lvgl_port_init(&port_cfg);
     if (err != ESP_OK)
```

### 5.2 修改 `app/app_ui.c` (异步 UI 调度)

```diff
@@ -13,11 +13,44 @@
 
 static const char *TAG = "app_ui";
 
+static char s_next_demo_name[32] = {0};
+
+/**
+ * @brief UI 切换的异步回调函数，将在 UI 线程 (taskLVGL) 上下文中被安全执行
+ */
+static void async_show_demo_cb(void *user_data)
+{
+    const char *demo_name = (const char *)user_data;
+    ESP_LOGI(TAG, "[Async Callback] 开始执行 UI 切换，目标 Demo: %s", demo_name);
+
+    // 1. 清空当前活动屏幕上的所有子对象 (在 UI 线程内执行绝对安全)
+    lv_obj_clean(lv_screen_active());
+
+    // 2. 根据目标名称加载 Demo
+    if (strcmp(demo_name, "widgets") == 0)
+    {
+        ...
+    }
+    else if (strcmp(demo_name, "benchmark") == 0)
+    {
+        ...
+    }
+    // ... 其他 Demo 的加载保持原逻辑 ...
+    
+    ESP_LOGI(TAG, "[Async Callback] UI 切换完成，目标: %s", demo_name);
+}
+
 void app_ui_show_demo(const char *demo_name)
 {
-    // 1. 锁定 LVGL 互斥锁以安全清理和配置 UI 控件 (多任务防冲突保护)
+    // 1. 锁定 LVGL 互斥锁以安全写入异步队列
     if (lvgl_port_lock(0))
     {
-        ESP_LOGI(TAG, "成功获取 UI 互斥锁，正在切换到 Demo: %s", demo_name);
-
-        // 清空当前活动屏幕上的所有子对象
-        lv_obj_clean(lv_screen_active());
-
-        // ... 同步加载 Demo 逻辑 ...
-
-        // 绘制完毕，解锁释放给 UI 运行引擎
-        lvgl_port_unlock();
+        // 2. 将目标 Demo 名称拷贝至静态缓冲区以确保异步回调时生命周期有效
+        strncpy(s_next_demo_name, demo_name, sizeof(s_next_demo_name) - 1);
+        s_next_demo_name[sizeof(s_next_demo_name) - 1] = '\0';
+
+        // 3. 注册异步操作，等待 UI 任务在其周期中执行
+        lv_async_call(async_show_demo_cb, s_next_demo_name);
+
+        // 4. 立即解锁返回，不阻塞当前控制台线程
+        lvgl_port_unlock();
+        ESP_LOGI(TAG, "UI 切换指令 '%s' 已成功排入异步队列", demo_name);
     }
```

---

## 6. 验证与验收结论

1. **编译测试**：执行 `idf.py build` 顺利编译生成固件，无编译错误。
2. **多线程并发稳定性**：
   * 在 Wi-Fi 正常联网、蓝牙高频发送广播的背景下，通过控制台终端执行 `ui_demo widgets` 或 `ui_demo music`。
   * 命令行响应立即返回（秒级），无任何延迟卡顿。
   * 控制台任务未发生爆满占死，`IDLE0` 喂狗正常，未触发任何 Task Watchdog 报警。
3. **UI 切换平滑性**：
   * UI 刷新任务在 Core 1 上独立执行异步清屏与重建，画面过渡顺滑。
   * 旋转方向、大小端及电容触摸功能一切正常，在后台高网络负载下依然保持流畅交互。
