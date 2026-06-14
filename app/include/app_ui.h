#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 启动 UI 交互任务
 */
void app_ui_start(void);

/**
 * @brief 切换不同的 UI Demo
 * @param demo_name Demo 名称，如 "widgets", "music", "benchmark", "stress" 等
 */
void app_ui_show_demo(const char *demo_name);

#ifdef __cplusplus
}
#endif
