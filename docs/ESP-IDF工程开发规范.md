# SZP_BSP项目 ESP-IDF 工程开发规范

> 版本：1.1.0  
> 更新日期：2026-06-11  
> 适用平台：ESP-IDF v5.5.1 (ESP32-S3)  
> 参考来源：ESP-IDF 官方编程指南

---

## 1. 核心设计与分层架构

ESP-IDF 工程采用多级组件化分层开发，以实现最大程度的驱动复用与应用层解耦。

### 1.1 架构层次划分与工程目录结构

项目目录树如下所示：

```
MyProject/
├── CMakeLists.txt            # 顶层构建配置，声明 EXTRA_COMPONENT_DIRS
├── sdkconfig.defaults        # 项目默认配置固化
├── main/                     # 应用层入口（IDF默认要求，可极简化）
│   ├── CMakeLists.txt
│   └── main.c                # 仅调用 app_init() 启动应用层核心逻辑
├── app/                      # 应用层核心逻辑（平级组件）
│   ├── CMakeLists.txt
│   ├── Kconfig               # 应用级全局配置（如业务开关）
│   ├── include/              # 对外暴露的应用层接口（供main或跨任务调用）
│   │   └── app_ui.h
│   └── app_ui.c
├── services/                 # 服务层/中间件（平级组件）
│   ├── CMakeLists.txt        # 聚合管理子服务（如WiFi、MQTT、OTA）
│   ├── Kconfig               # 服务组件配置菜单
│   ├── wifi_service/
│   │   ├── include/
│   │   │   └── wifi_service.h
│   │   └── wifi_service.c
│   └── mqtt_service/
├── bsp/                      # 板级支持包与硬件集成组件（平级组件）
│   ├── CMakeLists.txt        # 聚合管理板级抽象逻辑与初始化接口
│   ├── Kconfig.projbuild     # 板级引脚与硬件配置（全局可见）
│   ├── backlight/
│   ├── key/
│   └── i2c/
├── components/               # 自研可复用芯片驱动与基础功能组件（ESP-IDF默认路径，自动注册）
│   └── pca9557/              # 纯 PCA9557 芯片驱动组件（与开发板引脚解耦，仅依赖 I2C 句柄）
│       ├── CMakeLists.txt
│       ├── pca9557.c
│       └── include/
│           └── pca9557.h
├── managed_components/       # 乐鑫官方或第三方托管组件（自动生成，勿手动改）
└── build/                    # 编译输出产物（不纳入Git）
```

各层级职责划分：
* **入口层 (main)**：只包含系统启动引导代码（如 `app_main`），负责全局基础环境的初始化（如 NVS、Flash 等），并调用 `app_init()` 启动应用层核心逻辑。
* **应用层 (app)**：业务逻辑的核心位置，实现各种软件交互逻辑（如 UI、背词管理、拼写判定），通过调用 `services/` 或 `bsp/` 提供的接口，实现应用逻辑与底层硬件及通信协议的解耦。
* **服务层 (services)**：包含系统中间件与通信服务（如 Wi-Fi 连接、MQTT 通信、OTA 固件升级），通过 CMake 聚合管理这些子服务。
* **板级支持层 (bsp)**：负责封装板级的硬件引脚映射、通信初始化与高层接口（如 `bsp_key`、`bsp_backlight`、`bsp_i2c`）。它持有开发板引脚图纸的“硬编码”信息，通过整合底层的纯芯片驱动（来自 `components/`）实现具体的板级控制，并对上层隐藏 GPIO 细节。
* **自研芯片与公共组件层 (components)**：专门存放**与特定物理开发板完全解耦**的纯芯片驱动（如 `pca9557`、`gt911`、`es8311`）或者算法公共库。驱动代码仅依赖 ESP-IDF 标准驱动接口（如传入 I2C 或者是 SPI 句柄），内部禁止包含任何特定的引脚宏或硬编码硬件配置，可以直接复制移植到任何其他 ESP-IDF 工程中复用。
* **第三方管理组件 (managed_components)**：通过 `idf_component.yml` 自动下载的成熟官方及社区开源驱动包，由关理器管理，不做二次修改。

### 1.2 编译目录注册规范
为了支持根目录的 `app`、`services` 以及 `bsp` 被 ESP-IDF 正确编译，必须在项目根目录下的 `CMakeLists.txt` 中显式指定 `EXTRA_COMPONENT_DIRS` 注册这三个自定义目录：
```cmake
cmake_minimum_required(VERSION 3.16)

# 将自定义顶层文件夹注册为 ESP-IDF 组件搜索路径（注意：根目录下的 components/ 是默认搜索路径，无需在此处注册）
set(EXTRA_COMPONENT_DIRS "app" "services" "bsp")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(SZP_BSP)
```

### 1.3 驱动与外部组件引入原则

为了保证开发效率与代码健壮性，**拒绝重复造轮子**。针对任何外设元器件，必须遵循以下优先级来引入驱动与组件：

#### 1.3.1 引入优先级 (自顶向下)
1. **第一优先级：ESP Component Registry (官方组件库)**
   - 优先通过 ESP-IDF 包管理器（IDF Component Manager）引入。在组件目录下创建 `idf_component.yml` 文件，声明对官方仓库组件的依赖（例如 `espressif/esp_lcd_st7789`）。由构建系统自动下载和管理生命周期。
2. **第二优先级：社区成熟开源方案**
   - 若官方组件库中没有，优先选择 GitHub 活跃度高、经过验证的开源驱动库（例如 LibDriver、芯片厂商官方库等）。
   - **导入规范**：应将开源方案下载并放置于项目中合适的独立组件中（例如放置在 `bsp/` 下独立的驱动目录），并保持其源码原貌以利于后期跟线升级；绝不能将其源码零散地混入 `main/` 或本地业务组件中。
3. **第三优先级：自研开发**
   - 仅在官方及社区均无可用、或已有方案存在无法解决的致命缺陷时，方可自研驱动。自研芯片驱动命名必须遵循本规范，不带任何额外前缀，以芯片型号直接命名。

---

## 2. 构建系统与配置管理规范

### 2.1 CMakeLists.txt 规范

所有组件的 `CMakeLists.txt` 必须声明源文件路径、头文件包含路径以及依赖关系，遵循最小依赖原则。

#### 2.1.1 组件注册结构 (推荐使用目录自动收集)
为了避免后续每新增一个 `.c` 源文件都必须手动修改 `CMakeLists.txt`，组件注册统一**推荐使用 `SRC_DIRS` 参数自动收集源文件**目录，省去后续的手动维护成本：
```cmake
idf_component_register(
    SRC_DIRS "src"
    INCLUDE_DIRS "include"
    REQUIRES driver
    PRIV_REQUIRES esp_timer
)
```
> [!NOTE]
> 使用 `SRC_DIRS` 自动收集后，当您向组件 `src/` 目录下添加或删除了 `.c` 文件，CMake 会在下一次编译时自动发现新文件。若未自动识别，只需在终端运行 `idf.py reconfigure` 或在 IDE 中触发重新配置即可。

#### 2.1.2 方案对比与决策

在管理 ESP-IDF 的 CMakeLists.txt 时，常见的方案有以下三种。我们选择**方案二**：

| 组织方案 | 实现方式 | 优点 | 缺点 | 结论 |
| :--- | :--- | :--- | :--- | :--- |
| **方案一：组件内手动指定 `SRCS`** | 手动列出所有文件：<br>`SRCS "src/a.c" "src/b.c"` | 编译范围极其精准，依赖追踪最清晰。 | 极繁琐。每次添加、重命名源文件都必须修改 `CMakeLists.txt`。 | ❌ 不推荐 |
| **方案二：组件内使用 `SRC_DIRS`** | 自动收集目录：<br>`SRC_DIRS "src"` | **一劳永逸。** 后续向组件添加驱动文件时无需维护 CMake，同时保持了组件物理隔离与独立移植性。 | 新建文件后，极少数情况下需要手动执行 `idf.py reconfigure`。 | **✅ 推荐采用** |
| **方案三：只在 `main` 级集中维护** | 所有文件直接扔进 `main/`，仅在 `main/CMakeLists.txt` 中统一列出。 | 只有一个文件，短期内看起来省心。 | **完全破坏了组件的模块化隔离**。头文件空间被打平，极易造成头文件滥用、循环依赖，模块无法单独移植。 | ❌ 严厉禁止 |

#### 2.1.3 依赖管理声明
- **`REQUIRES` (公有依赖)**：如果组件的头文件（`.h`）中引入了其他组件的头文件，必须将其放入 `REQUIRES`。
- **`PRIV_REQUIRES` (私有依赖)**：如果仅在组件的源文件（`.c`）中引入了其他组件的头文件，必须将其放入 `PRIV_REQUIRES` 以减少编译依赖链。

### 2.2 Kconfig 分层配置管理规范

为了支持通过 `idf.py menuconfig` 动态修改项目参数与进行模块开关，Kconfig 采用分层结构进行管理：

#### 2.2.1 应用层配置 (app/Kconfig)
* **原则**：定义应用级别的全局功能、业务开关及默认业务常数（如是否启用调试控制台 `esp_console`、云端服务器 URL、应用初始音量等）。
* **呈现位置**：出现在 `menuconfig` 的 `Component config -> app` 菜单下。

#### 2.2.2 服务层配置 (services/Kconfig)
* **原则**：配置服务层中间件参数（如 Wi-Fi 重连次数、MQTT 维持连接时间、OTA 服务器端口等）。
* **呈现位置**：出现在 `menuconfig` 的 `Component config -> services` 菜单下。

#### 2.2.3 板级引脚与硬件配置 (bsp/Kconfig.projbuild)
* **原则**：定义开发板本身的硬件引脚配置、背光控制模式选择、音频外设时钟等硬件强相关参数。使用 `Kconfig.projbuild` 使其配置项对所有组件全局可见。
* **呈现位置**：直接以顶级主菜单形式呈现在 `menuconfig` 的根目录下，便于开发者修改引脚与基本硬件定义。

#### 2.2.4 命名空间规范
所有 Kconfig 配置选项必须遵守命名空间前缀限制以防变量污染：
- 板级硬件/引脚配置项前缀：`config BSP_`（如 `BSP_LED_GPIO_NUM`）
- 服务层配置项前缀：`config <SERVICE_NAME>_`（如 `WIFI_SERVICE_RECONNECT_MAX`）
- 应用层配置项前缀：`config APP_`（如 `APP_DEFAULT_VOLUME`）
- 纯芯片驱动配置项前缀：`config <CHIP_NAME>_`（如 `PCA9557_I2C_SPEED`）

### 2.3 配置基准管理 (`sdkconfig.defaults`)

- 由于本地编译生成的 `sdkconfig` 包含用户本地环境差异且已被列入 `.gitignore`，**项目基准配置必须保存在 `sdkconfig.defaults` 中**。
- 凡是影响核心硬件属性（如 Flash 大小、CPU 频率、PSRAM 启用、日志级别）的配置，均应在修改后将其同步更新至 `sdkconfig.defaults` 并提交至 Git。

---

## 3. 命名规范

### 3.1 文件命名

- **应用层文件**：`app_<功能>.c/h`（如 `app_ui.c`、`app_init.c`）
- **服务层文件**：`<服务名>.c/h`（如 `wifi_service.c`、`mqtt_service.c`）
- **板级抽象/底层驱动**：`bsp_<外设>.c/h`（如 `bsp_led.c`、`bsp_key.c`）
- **自研芯片驱动**：直接以芯片型号命名，无任何前缀（如 `pca9557.c`、`st7789.c`）

### 3.2 函数命名

- **应用层函数**：`app_<模块>_<动作>`（如 `app_ui_start()`）
- **服务层函数**：`<服务名>_<动作>`（如 `wifi_service_connect()`）
- **板级服务函数**：`bsp_<外设>_<动作>`（如 `bsp_led_on()`、`bsp_key_read()`）
- **芯片驱动函数**：`<芯片>_<动作>`（如 `pca9557_init()`）
- **私有函数**：`static` 声明并使用 `s_` 前缀（如 `static void s_pca9557_write_reg()`）

### 3.3 变量与宏定义

- **局部变量**：`snake_case`，如 `uint16_t pixel_count;`
- **全局变量**：`g_` 前缀，如 `uint8_t g_system_state;`
- **静态变量**：`s_` 前缀，如 `static uint16_t s_lcd_width;`
- **指针变量**：`p_` 前缀，如 `uint8_t *p_data;`
- **宏定义**：全大写 + 下划线，如 `#define LCD_WIDTH (240U)`。
- **无符号整型常量**：必须使用 `U` 后缀，如 `#define BIT(n) (1U << (n))`。
- **函数式宏**：参数和整体必须加括号，如 `#define MAX(a, b) ((a) > (b) ? (a) : (b))`。

---

## 4. 代码格式

遵循项目根目录 `.clang-format` 配置文件规范：
- **缩进**：4 空格，禁止使用 Tab。
- **大括号**：Allman 风格（大括号独占一行换行）。
- **单行限制**：最大 120 字符。
- **注释分隔符**：用于划分逻辑块，保持 64 个 `=` 长度。
  ```c
  /* ================================================================
   * LCD 初始化函数
   * ================================================================ */
  ```
- **注释规范**：
  * **公开头文件（`*.h`）**：对外暴露的 API 函数、结构体、枚举，**必须使用 Doxygen 格式注释**（以 `/**` 开头），并写明 `@brief`、`@param` 和 `@return` 等标签，以确保自动生成 API 手册以及在 IDE 中获得完美的悬停提示。
  * **实现源文件（`*.c`）**：源码中必须包含清晰、规范的注释。具体规则如下：
    * **大分类/大逻辑块划分**：必须使用 64 个 `=` 长度的注释分隔符进行物理区隔：
      ```c
      /* ================================================================
       * 核心控制台命令注册逻辑
       * ================================================================ */
      ```
    * **重要说明与多步操作**：对于重要的背景解释、系统逻辑说明或有明显顺序步骤要求的内容，**必须**使用块注释 `/* ... */` 格式独占一行书写。
    * **语句后尾随说明**：禁止在代码语句后面使用块注释 `/* ... */`，语句末尾的简短说明必须统一使用双斜线 `//` 尾随。
    * **对外函数实现**：对外公开函数在 `*.c` 文件中的实现上方，也必须使用 Doxygen（`/** ... */`）或必要的说明注释，但**无需重复书写** `@param` 和 `@return` 标签（IDE 会自动从头文件索引，此处保持简洁描述即可）。
    * **本地辅助/静态函数**：本地静态函数（`static`）上方必须使用简单的块注释 `/* ... */` 进行基础的功能与用途描述。

---

## 5. 错误处理规范

### 5.1 错误返回值

所有涉及初始化、配置、I/O 读写的公开函数统一返回 **`esp_err_t`**。
- 成功返回 `ESP_OK` (0)。
- 失败返回标准错误码（如 `ESP_ERR_NO_MEM`、`ESP_ERR_INVALID_ARG` 等）。

### 5.2 错误处理模式 (基于 `esp_check.h`)

使用 `esp_check.h` 提供的宏简化错误处理逻辑，杜绝冗余的 if-else：
- **核心初始化失败检查**（无法继续运行的硬件配置故障）：使用 `ESP_ERROR_CHECK(err)` 进行复位断言。
- **常规运行时错误提前返回**：
  ```c
  esp_err_t ret = spi_bus_initialize(...);
  ESP_RETURN_ON_ERROR(ret, TAG, "SPI bus initialization failed");
  ```
- **资源清理与错误跳转**：
  ```c
  esp_err_t ret = malloc_buffer();
  ESP_GOTO_ON_ERROR(ret, cleanup, TAG, "Failed to allocate memory");
  ...
  cleanup:
      free_buffer();
  ```

---

## 6. 日志与调试输出规范

### 6.1 日志输出与 printf 使用界限

- **组件开发与后台逻辑**：**绝对禁止**使用 `printf` 打印调试信息，必须使用 `esp_log.h` 接口。
- **格式化排版控制台**：在打印 CLI 菜单、ASCII 图案、数据对齐表格等需要精确排版控制的文本流中，**必须使用 `printf`**，避免日志元数据前缀破坏格式。

### 6.2 `ESP_LOGx` 规范

每个源文件必须在顶部定义局部静态常量 `TAG`，并在调用时传入该 Tag：
```c
static const char *TAG = "bsp_lcd";
```
日志级别选用标准：
- `ESP_LOGE`（错误）：硬件异常、超时、可能导致模块停机的问题。
- `ESP_LOGW`（警告）：不影响正常运行的边界异常（如队列溢出数据被覆盖）。
- `ESP_LOGI`（信息）：展示关键模块初始化成功、系统阶段切换。
- `ESP_LOGD`（调试）：展示协议层数据传输详情、参数校验。

---

## 7. FreeRTOS 多任务与事件驱动规范

项目深度基于 FreeRTOS 运行，逻辑调度必须符合操作系统的并发契约。

### 7.1 FreeRTOS 任务生命周期与分配

- **创建任务**：统一使用 `xTaskCreatePinnedToCore` 将任务绑定到指定 CPU 核心。任务优先级和堆栈大小应在组件头文件中统一声明，严禁硬编码。
- **任务安全销毁**：包含退出路径的任务，**在退出函数体前必须调用 `vTaskDelete(NULL)`**，绝对禁止直接 `return`。

### 7.2 事件驱动状态机 (Event-driven FSM)

- **事件接收**：状态机主调度任务持有 FreeRTOS 队列（`QueueHandle_t`）。
- **非阻塞事件投递**：在按键中断回调或定时器回调中，只向事件队列发送事件类型及数值，禁止在回调内直接处理耗时业务。
- **事件等待与消费**：主任务通过 `xQueueReceive(xQueue, &event, portMAX_DELAY)` 挂起等待事件，通过 `switch(state)` 进行状态流转，必须包含 `default:` 兜底警告。

### 7.3 周期性无阻塞定时器

- 避免使用任务内部延时（`vTaskDelay`）进行耗时周期的状态轮询。
- 对于心跳检测、呼吸灯控制、按键消抖轮询等周期性轻量操作，应注册 ESP-IDF 的 **`esp_timer`** 硬件高精度软件定时器，通过回调函数驱动。
