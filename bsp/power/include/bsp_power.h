/**
 * @file bsp_power.h
 * @brief 板级电源与低功耗管理
 * @version 1.0
 * @date 2026-06-13
 */

#ifndef BSP_POWER_H
#define BSP_POWER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 板级支持的低功耗休眠模式
 */
typedef enum {
    BSP_SLEEP_MODE_LIGHT,   /*!< 轻度睡眠模式 */
    BSP_SLEEP_MODE_DEEP,    /*!< 深度睡眠模式 */
} bsp_sleep_mode_t;

/**
 * @brief 初始化板级电源管理（包含动态频率缩减 DFS 以及自动轻度睡眠）
 * 
 * @return 
 *      - ESP_OK: 成功
 *      - ESP_ERR_NOT_SUPPORTED: sdkconfig 中未启用电源管理
 *      - 其他 esp_err_t: 配置失败错误码
 */
esp_err_t bsp_power_init(void);

/**
 * @brief 配置 BOOT 物理按键 (GPIO0) 作为唤醒源并进入指定休眠模式
 * 
 * @param mode 休眠模式 (轻度睡眠或深度睡眠)
 * @return 
 *      - ESP_OK: 成功 (轻度睡眠唤醒后正常返回 ESP_OK，深度睡眠唤醒后复位重启不返回)
 *      - 其他 esp_err_t: 休眠/唤醒源配置失败错误码
 */
esp_err_t bsp_power_enter_sleep(bsp_sleep_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif // BSP_POWER_H
