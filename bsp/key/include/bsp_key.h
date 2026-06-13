#pragma once

/**
 * @file bsp_key.h
 * @brief 板载按键驱动头文件，基于 iot_button 组件封装用户输入接口
 */

#include <stdbool.h>
#include "esp_err.h"
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

// 声明按键事件基 (EVENT BASE)
ESP_EVENT_DECLARE_BASE(BSP_KEY_EVENT_BASE);

/**
 * @brief 板载按键事件 ID 定义
 */
typedef enum {
    BSP_KEY_EVENT_SINGLE_CLICK, ///< 单击事件
    BSP_KEY_EVENT_DOUBLE_CLICK, ///< 双击事件
    BSP_KEY_EVENT_MAX
} bsp_key_event_id_t;

/**
 * @brief 初始化板载按键，配置 GPIO 模式并准备按键设备
 */
void bsp_key_init(void);

/**
 * @brief 读取按键实时物理状态
 * @return true 表示按键处于按下状态（物理低电平），false 表示未按下
 */
bool bsp_key_is_pressed(void);

#ifdef __cplusplus
}
#endif
