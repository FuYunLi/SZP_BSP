#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载按键
 */
void bsp_key_init(void);

/**
 * @brief 读取按键状态
 * @return true 表示按下，false 表示未按下
 */
bool bsp_key_is_pressed(void);

#ifdef __cplusplus
}
#endif
