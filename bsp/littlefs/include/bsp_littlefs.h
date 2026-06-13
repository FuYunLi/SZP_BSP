/**
 * @file bsp_littlefs.h
 * @brief 板级支持包内部 Flash LittleFS 文件系统接口
 */

#ifndef BSP_LITTLEFS_H
#define BSP_LITTLEFS_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 挂载片上 Flash 的 LittleFS 分区
 * 
 * @return 
 *      - ESP_OK: 挂载成功
 *      - ESP_FAIL: 挂载或注册 VFS 失败
 *      - 其他标准错误码
 */
esp_err_t bsp_littlefs_mount(void);

/**
 * @brief 卸载 LittleFS 分区
 * 
 * @return 
 *      - ESP_OK: 卸载成功
 *      - 其他标准错误码
 */
esp_err_t bsp_littlefs_unmount(void);

/**
 * @brief 检查 LittleFS 是否已挂载
 * 
 * @return true 已挂载, false 未挂载
 */
bool bsp_littlefs_is_mounted(void);

#ifdef __cplusplus
}
#endif

#endif // BSP_LITTLEFS_H
