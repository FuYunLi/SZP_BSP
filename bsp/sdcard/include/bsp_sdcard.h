/**
 * @file bsp_sdcard.h
 * @brief 板级支持包外部 MicroSD 卡接口
 */

#ifndef BSP_SDCARD_H
#define BSP_SDCARD_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 挂载外部 MicroSD 卡到 /sdcard
 * 
 * @return 
 *      - ESP_OK: 挂载成功
 *      - 其他标准错误码
 */
esp_err_t bsp_sdcard_mount(void);

/**
 * @brief 卸载 MicroSD 卡
 * 
 * @return 
 *      - ESP_OK: 卸载成功
 *      - 其他标准错误码
 */
esp_err_t bsp_sdcard_unmount(void);

/**
 * @brief 检查 MicroSD 卡是否已挂载
 * 
 * @return true 已挂载, false 未挂载
 */
bool bsp_sdcard_is_mounted(void);

#ifdef __cplusplus
}
#endif

#endif // BSP_SDCARD_H
