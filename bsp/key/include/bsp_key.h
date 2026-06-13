#pragma once

/**
 * @file bsp_key.h
 * @brief 板载按键驱动头文件，基于 iot_button 组件封装用户输入接口
 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载按键，配置 GPIO 模式并注册按键行为回调
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
