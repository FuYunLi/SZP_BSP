#ifndef BSP_TOUCH_H
#define BSP_TOUCH_H

#include "esp_err.h"
#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载电容触摸驱动 (FT5x06)
 * 
 * @return esp_err_t ESP_OK 表示成功，其他表示失败
 */
esp_err_t bsp_touch_init(void);

/**
 * @brief 读取触摸点坐标与数量
 * 
 * @param[out] x 触控点 X 坐标 (若未触摸则保持原值或设为 0)
 * @param[out] y 触控点 Y 坐标 (若未触摸则保持原值或设为 0)
 * @param[out] points_num 触控点数 (0 表示松开)
 * @return esp_err_t ESP_OK 表示读取成功，其他表示失败
 */
esp_err_t bsp_touch_read(uint16_t *x, uint16_t *y, uint8_t *points_num);

/**
 * @brief 获取触摸驱动句柄
 * 
 * @return esp_lcd_touch_handle_t 触摸屏句柄
 */
esp_lcd_touch_handle_t bsp_touch_get_handle(void);

#ifdef __cplusplus
}
#endif

#endif // BSP_TOUCH_H
