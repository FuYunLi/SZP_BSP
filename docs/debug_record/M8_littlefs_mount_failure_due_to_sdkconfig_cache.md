# M8_LittleFS 挂载失败与 sdkconfig 缓存冲突调试记录

## 1. 问题描述
在实现板级 LittleFS 局部文件系统挂载（M8 阶段）后，将固件烧录进开发板运行，串口监视器陷入无限重启循环（Reset Loop），输出以下关键错误日志：
```text
I (829) bsp_littlefs: Mounting LittleFS partition 'storage'...
E (829) esp_littlefs: partition "storage" could not be found
E (829) esp_littlefs: Failed to initialize LittleFS
E (839) bsp_littlefs: Failed to find LittleFS partition 'storage'
ESP_ERROR_CHECK failed: esp_err_t 0x105 (ESP_ERR_NOT_FOUND) at 0x4200d3cf
--- 0x4200d3cf: app_init at /home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/app_init.c:62
file: "./app/app_init.c" line 62
func: app_init
expression: bsp_littlefs_mount()

abort() was called at PC 0x4037be2f on core 0
```
分析表明，系统在启动时尝试挂载名为 `"storage"` 的 LittleFS 分区，但 VFS 适配层报告找不到该分区（`ESP_ERR_NOT_FOUND`），导致系统断言失败并不断重启。

---

## 2. 问题排查与原因定位过程

### 步骤一：检查 Bootloader 分区表打印
观察系统刚上电时，二级引导程序（2nd stage bootloader）打印的实际分区表结构：
```text
I (47) boot: Partition Table:
I (50) boot: ## Label            Usage          Type ST Offset   Length
I (56) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (62) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (69) boot:  2 factory          factory app      00 00 00010000 00100000
I (76) boot: End of partition table
```
*   **分析**：实际被烧录进 Flash 的分区表仍然是 ESP-IDF 默认的 **单 App 结构（Single App）**，根本没有我们新规划的 `storage`、`ota_0` 和 `ota_1` 分区。
*   **疑问**：我们明明在根目录创建了 `partitions.csv` 且在 `sdkconfig.defaults` 中添加了自定义分区表启用宏，为什么编译生成的固件没有应用这些配置？

### 步骤二：核对 `sdkconfig` 实际配置（核心问题）
使用 Grep 工具对项目根目录下实际生效的 `sdkconfig` 配置文件进行检索：
```bash
grep "CONFIG_PARTITION_TABLE" sdkconfig
```
检索输出结果为：
```text
CONFIG_PARTITION_TABLE_SINGLE_APP=y
# CONFIG_PARTITION_TABLE_CUSTOM is not set
CONFIG_PARTITION_TABLE_FILENAME="partitions_singleapp.csv"
```
*   **原因分析**：
    1.  **ESP-IDF 的配置覆盖规则**：在构建系统时，如果 `sdkconfig` 缓存文件已经存在，CMake 构建脚本**不会**自动把 `sdkconfig.defaults` 中的新修改项合并或应用到已有的 `sdkconfig` 中。
    2.  我们此前在 M7 阶段已经编译过项目，因此根目录下残留了旧的 `sdkconfig` 文件。我们直接修改 `sdkconfig.defaults` 后再次执行 `idf.py build`，构建系统依然使用旧的 `sdkconfig` 进行编译，导致生成的 `partition-table.bin` 仍然是旧的单 App 布局。

---

## 3. 解决办法

为了彻底清除 `sdkconfig` 缓存以应用自定义分区表，执行了以下步骤：

1.  **删除旧的 `sdkconfig` 文件**：
    ```bash
    rm sdkconfig
    ```
2.  **重新执行编译生成新配置**：
    ```bash
    idf.py build
    ```
    *构建系统检测到 `sdkconfig` 不存在，会从零读取 `sdkconfig.defaults`，成功生成了激活 `CONFIG_PARTITION_TABLE_CUSTOM=y` 和 `partitions.csv` 的新 `sdkconfig` 并完成了编译。*
3.  **整片擦除并重新烧录**：
    由于分区表物理偏移（特别是 `factory` 分区起始偏移）发生了剧烈变化，为防止残留的旧数据影响引导，执行全片擦除再重新烧录：
    ```bash
    idf.py erase-flash
    idf.py flash monitor
    ```

---

## 4. 验证结果

重新擦除烧录后，开发板输出正常的分区表和初始化日志：

```text
I (48) boot: Partition Table:
I (50) boot: ## Label            Usage          Type ST Offset   Length
I (56) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (63) boot:  1 otadata          OTA data         01 00 0000f000 00002000
I (70) boot:  2 phy_init         RF data          01 01 00011000 00001000
I (76) boot:  3 factory          factory app      00 00 00020000 00300000
I (83) boot:  4 ota_0            OTA app          00 10 00320000 00300000
I (89) boot:  5 ota_1            OTA app          00 11 00620000 00300000
I (96) boot:  6 storage          Unknown data     01 83 00920000 00400000
I (102) boot: End of partition table
...
I (835) bsp_littlefs: Mounting LittleFS partition 'storage'...
E (835) esp_littlefs: ./managed_components/joltwallet__littlefs/src/littlefs/lfs.c:1383:error: Corrupted dir pair at {0x0, 0x1}
W (845) esp_littlefs: mount failed,  (-84). formatting...
I (855) bsp_littlefs: LittleFS partition 'storage' mounted successfully at '/littlefs'
```
*   **分析**：首次启动时由于 Flash 为空，LittleFS 挂载失败并触发自动格式化，格式化后成功挂载至 `/littlefs`。
*   **指令测试**：
    在命令行终端调用我们编写的 `fs_write` 和 `fs_read` 工具：
    ```text
    SZP> fs_write test.txt "Hello_LittleFS_M8"
    Successfully wrote 17 bytes to file /littlefs/test.txt
    SZP> fs_read test.txt
    Content of file /littlefs/test.txt:
    Hello_LittleFS_M8
    ```
    重启开发板后，再次运行 `fs_read test.txt` 内容依然存在，证明板级 LittleFS 局部文件持久化系统正常工作。

---

## 5. 经验与启示
1.  **关于 `sdkconfig.defaults`**：一旦修改了 `sdkconfig.defaults`（特别是由默认分区表改为自定义分区表等底层核心参数时），必须**先删除工作区根目录的 `sdkconfig` 文件**再编译，或者手动通过 `idf.py menuconfig` 设置，否则改动无法合入。
2.  **关于分区表偏移改变**：当分区表布局改变时，千万不要只烧录 `app` 部分，必须使用 `idf.py erase-flash` 先擦除整片，然后再使用 `idf.py flash` 进行完整烧录，以确保 Bootloader、分区表和 App 偏移均对齐写入且旧数据被清空。
