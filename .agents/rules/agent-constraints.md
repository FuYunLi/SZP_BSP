# Agent 行为约束规范 (ESP-IDF 版)

> 版本：1.0.0  
> 更新日期：2026-06-11  
> 用途：约束 AI Agent 在本项目中的行为，防止误操作并确保开发安全

---

## 1. 权限分级

| 权限级别 | 允许操作 | 禁止操作 | 适用场景 |
|---------|---------|---------|---------|
| **只读** | 读取文件、搜索代码 | 任何写入操作 | 问题分析与调研 |
| **本地开发** | 本地修改、本地编译测试 | 推送到远程 | 功能开发中 |
| **完全权限** | 所有操作 | 无 | **仅限人工** |

---

## 2. 禁止操作清单

### 2.1 绝对禁止（任何权限级别）

```bash
# 禁止直接推送到远程受保护分支
git push origin master
git push origin main

# 禁止强制推送
git push --force
git push -f

# 禁止删除远程分支
git push --delete origin <branch>

# 禁止危险清理命令
git clean -fdx
```

### 2.2 需要用户明确确认的操作

```bash
# 提交代码 (禁止 Agent 擅自 commit)
git commit

# 切换/创建分支
git checkout -b <name>

# 合并/变基操作
git merge / git rebase
```

---

## 3. 工作流程约束

### 3.1 权限与确认原则

1. **禁止自主提交**：Agent **严禁自主发起 `git commit`**。必须由用户发起或得到明确指令（如“请提交当前修改”），且在提交前必须执行 `git status` 及展示 `git diff --stat` 供用户审查确认。
2. **提交日志规范**：提交日志（Git commit message）必须全部使用**中文**编写。必须采用约定式提交（Conventional Commits）结构（例如：`feat(board): 新增 bsp_lcd 驱动`），并在正文中说明**设计意图**、**变更细节**与**技术细节**。
3. **停止与确认**：遇到任何有歧义的开发方案或编译报错，必须立即停止并询问用户，禁止静默处理或连续盲目修改代码尝试修复。

### 3.2 ESP-IDF 编译、烧录与调试工作流

为了防范由于环境变量丢失（如未执行 `export.sh`）或多平台编译工具差异导致的编译/烧录阻断，并提供结构化的错误解析与自愈，**Agent 在需要执行构建、烧录、调试或监控时，应优先调用工程内置的专用 Skills 进行调用**：

#### 1. 编译规范 (优先调用 `build-idf` 技能)
- Agent 编译或切换目标芯片时，应优先使用 `build-idf` 技能中自带的底层脚本进行调用，以保证环境自动激活：
  - 探测环境：`python3 .agents/skills/build-idf/scripts/idf_builder.py --detect`
  - 设置芯片：`python3 .agents/skills/build-idf/scripts/idf_builder.py --set-target <target> --project <path>`
  - 编译工程：`python3 .agents/skills/build-idf/scripts/idf_builder.py --build --project <path>`
- 只有在上述脚本报环境故障或发生无法自动处理的底层异常时，方可在用户授权后在已激活的环境中使用原生命令行 `idf.py build` 执行补充操作。
- **编译结果汇报**：
  - 编译成功时：汇报固件大小（ELF/BIN）以及芯片目标。
  - 编译失败时：展示结构化的 `failure_category` 与关键出错日志，**禁止盲目修改代码重试**，应先向用户分析原因，获得确认后再修改。

#### 2. 烧录与调试规范 (优先调用 `flash-idf` / `serial-monitor` 技能)
- **烧录程序**：需优先通过 `flash-idf` 技能的 `idf_flasher.py` 进行烧录（例：`python3 .agents/skills/flash-idf/scripts/idf_flasher.py --flash --project <path> --port <port>`）。必须在获得用户授权后方可执行。
- **监控串口日志**：在获取运行日志时，应优先使用 `serial-monitor` 技能进行日志抓取与运行状态监控。在长连接调试中，也可配合使用 `idf.py monitor` 命令行。

---

## 4. 特殊场景防御性编程规范

### 4.1 FreeRTOS 任务与资源管理

在编写 ESP-IDF 多任务代码时，必须遵守以下要求：
1. **任务退出销毁**：任何 FreeRTOS 任务函数在退出前，必须调用 `vTaskDelete(NULL)`，严禁直接 `return` 导致内核崩溃。
2. **看门狗喂狗**：在耗时较长的死循环任务中，必须适当调用 `vTaskDelay` 或显式喂狗，防止触发看门狗复位。
3. **堆区内存释放**：动态分配的内存（如 `malloc`、`pvPortMalloc`）必须有明确的释放路径，优先使用 FreeRTOS 静态创建机制（如 `xTaskCreateStatic`）。

### 4.2 ESP-IDF 原生错误处理绑定

1. 初始化外设时，必须检查返回值是否为 `ESP_OK`。
2. 不允许静默忽略错误代码。必须使用 `ESP_ERROR_CHECK()` 对致命错误进行硬断言，或者使用 `ESP_RETURN_ON_ERROR()` 向上级传递。
