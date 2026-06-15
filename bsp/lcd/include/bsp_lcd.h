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

#include "esp_lcd_panel_io.h"

/**
 * @brief 注册 LCD SPI 传输完成中断回调 (面向 LVGL 异步刷新)
 * 
 * @param[in] cb 中断回调函数
 * @param[in] user_ctx 回调上下文参数 (一般为 lv_display_t 句柄)
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lcd_register_trans_done_cb(esp_lcd_panel_io_color_trans_done_cb_t cb, void *user_ctx);

#include "esp_lcd_panel_vendor.h"

/**
 * @brief 获取 LCD 面板驱动句柄
 * 
 * @return esp_lcd_panel_handle_t 面板句柄
 */
esp_lcd_panel_handle_t bsp_lcd_get_panel_handle(void);

/**
 * @brief 获取 LCD IO 驱动句柄
 * 
 * @return esp_lcd_panel_io_handle_t IO句柄
 */
esp_lcd_panel_io_handle_t bsp_lcd_get_io_handle(void);

/**
 * @brief 反初始化 LCD 驱动，释放所有资源
 * 
 * @note 此函数会释放 SPI 总线、删除面板驱动、释放清屏缓冲区
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_lcd_deinit(void);

#ifdef __cplusplus
}
#endif
