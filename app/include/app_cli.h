#pragma once

/**
 * @file app_cli.h
 * @brief 应用层命令行接口头文件，管理用户自定义控制台命令的注册与分发
 */

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化并注册应用层控制台命令
 * @return esp_err_t ESP_OK 表示启动成功，否则表示失败
 */
esp_err_t app_cli_init(void);

#ifdef __cplusplus
}
#endif