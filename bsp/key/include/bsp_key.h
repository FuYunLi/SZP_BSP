#pragma once

/**
 * @file bsp_key.h
 * @brief 板载按键驱动头文件，基于 iot_button 组件封装用户输入接口
 */

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 板载按键事件类型定义，解耦底层组件
 */
typedef enum {
    BSP_KEY_SINGLE_CLICK,       ///< 单击事件
    BSP_KEY_DOUBLE_CLICK,       ///< 双击事件
    BSP_KEY_EVENT_MAX
} bsp_key_event_t;

/**
 * @brief 按键事件回调函数类型定义
 */
typedef void (*bsp_key_cb_t)(void *arg, void *usr_data);

/**
 * @brief 初始化板载按键，配置 GPIO 模式并准备按键设备
 */
void bsp_key_init(void);

/**
 * @brief 读取按键实时物理状态
 * @return true 表示按键处于按下状态（物理低电平），false 表示未按下
 */
bool bsp_key_is_pressed(void);

/**
 * @brief 注册按键事件回调函数
 * @param event 按键事件类型
 * @param cb 用户自定义的回调函数指针
 * @param usr_data 用户私有数据指针
 * @return esp_err_t 注册结果，ESP_OK 表示成功
 */
esp_err_t bsp_key_register_callback(bsp_key_event_t event, bsp_key_cb_t cb, void *usr_data);

#ifdef __cplusplus
}
#endif
