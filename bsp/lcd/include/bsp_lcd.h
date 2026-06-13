/**
 * @file bsp_lcd.h
 * @brief 2.0 英寸 IPS 液晶显示屏 (ST7789) 板级支持包头文件
 */

#pragma once

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 屏幕分辨率宏定义 */
#define LCD_H_RES 320
#define LCD_V_RES 240

/**
 * @brief 初始化 LCD 驱动 (ST7789 via SPI and PCA9557 CS)
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lcd_init(void);

/**
 * @brief 填充整个 LCD 屏幕为指定颜色 (清屏)
 * @param[in] color 16位 RGB565 颜色值
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lcd_clear(uint16_t color);

/**
 * @brief 绘制位图到屏幕指定区域
 * @param[in] x_start 起始 X 坐标 (0 ~ LCD_H_RES-1)
 * @param[in] y_start 起始 Y 坐标 (0 ~ LCD_V_RES-1)
 * @param[in] x_end 结束 X 坐标 (1 ~ LCD_H_RES)
 * @param[in] y_end 结束 Y 坐标 (1 ~ LCD_V_RES)
 * @param[in] color_data 像素数据缓冲区
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lcd_draw_bitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data);

#ifdef __cplusplus
}
#endif
