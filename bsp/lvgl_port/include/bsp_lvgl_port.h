/**
 * @file bsp_lvgl_port.h
 * @brief LVGL 9.x 乐鑫官方组件适配移植层头文件
 */

#pragma once

#include "esp_err.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 LVGL 移植层 (基于 esp_lvgl_port，带显示与触控集成)
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lvgl_port_init(void);

/**
 * @brief 获取已初始化的显示设备句柄
 * 
 * @return lv_display_t* 显示设备指针，若未初始化则返回 NULL
 */
lv_display_t *bsp_lvgl_port_get_display(void);

/**
 * @brief 反初始化 LVGL 移植层，释放所有资源
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lvgl_port_deinit(void);

#ifdef __cplusplus
}
#endif
