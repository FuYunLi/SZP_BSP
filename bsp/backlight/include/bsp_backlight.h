#pragma once

/**
 * @file bsp_backlight.h
 * @brief 屏幕背光板级支持包头文件，提供 GPIO 控制接口
 */

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载屏幕背光引脚为输出模式
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_backlight_init(void);

/**
 * @brief 设置板载屏幕背光状态
 * @param[in] enable true 表示开启背光（输出低电平），false 表示关闭背光（输出高电平）
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_backlight_set(bool enable);

/**
 * @brief 反初始化板载屏幕背光引脚
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_backlight_deinit(void);

#ifdef __cplusplus
}
#endif
