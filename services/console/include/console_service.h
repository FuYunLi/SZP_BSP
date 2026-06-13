#pragma once

/**
 * @file console_service.h
 * @brief 控制台服务头文件，提供交互式终端的启动接口
 */

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化并启动控制台终端服务 (REPL)
 * @return esp_err_t ESP_OK 表示启动成功，否则表示失败
 */
esp_err_t console_service_start(void);

#ifdef __cplusplus
}
#endif
