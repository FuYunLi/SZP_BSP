/**
 * @file bsp_lcd.c
 * @brief 2.0 英寸 IPS 液晶显示屏 (ST7789) 板级支持包实现源文件
 */

#include "bsp_lcd.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "pca9557.h"
#include "esp_log.h"
#include <stdlib.h>

#define LCD_HOST               SPI2_HOST
#define PIN_NUM_LCD_DC         39
#define PIN_NUM_LCD_MOSI       40
#define PIN_NUM_LCD_CLK        41

static const char *TAG = "bsp_lcd";
static esp_lcd_panel_handle_t s_panel_handle = NULL;
static esp_lcd_panel_io_handle_t s_io_handle = NULL;
static bool s_lcd_initialized = false;

/**
 * @brief 初始化 LCD 硬件
 */
esp_err_t bsp_lcd_init(void)
{
    if (s_lcd_initialized)
    {
        ESP_LOGW(TAG, "LCD 驱动已初始化");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "正在初始化 LCD SPI 总线...");

    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_LCD_CLK,
        .mosi_io_num = PIN_NUM_LCD_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 20 * sizeof(uint16_t), // 最大单次传输 20 行像素
    };

    esp_err_t ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 SPI 总线失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在通过 PCA9557 配置 LCD_CS 引脚...");
    // 配置 PCA9557 Pin 0 (LCD_CS) 为输出，并拉低片选以使能屏幕 SPI 传输
    ret = pca9557_set_config(0, false);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "配置 PCA9557 扩展引脚为输出失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = pca9557_set_output_level(0, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "拉低 LCD_CS 失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在创建 LCD 面板 IO 句柄...");
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_LCD_DC,
        .cs_gpio_num = -1, // 片选由外部 PCA9557 手动管理，此处传入 -1
        .pclk_hz = 40 * 1000 * 1000, // 40MHz 像素时钟
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };

    ret = esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &s_io_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "创建 LCD IO 句柄失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在加载 ST7789 驱动...");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = -1, // 硬件复位直接连至系统 CHIP_PU，软件无需控制配置为 -1
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ret = esp_lcd_new_panel_st7789(s_io_handle, &panel_config, &s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "加载 ST7789 面板驱动失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在对 LCD 面板进行软复位和初始化...");
    ret = esp_lcd_panel_reset(s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "重置 LCD 面板失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_init(s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 LCD 面板失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 屏幕显示基本配置：开启显示、开启反色、坐标轴调换、Y轴镜像等
    esp_lcd_panel_disp_on_off(s_panel_handle, true);
    esp_lcd_panel_invert_color(s_panel_handle, true);
    esp_lcd_panel_swap_xy(s_panel_handle, true);
    esp_lcd_panel_mirror(s_panel_handle, false, true);

    s_lcd_initialized = true;
    ESP_LOGI(TAG, "LCD 驱动配置及初始化成功完成");
    return ESP_OK;
}

/**
 * @brief 填充整个 LCD 屏幕为指定颜色 (清屏)
 */
esp_err_t bsp_lcd_clear(uint16_t color)
{
    if (!s_lcd_initialized)
    {
        ESP_LOGE(TAG, "LCD 未初始化，无法执行清屏");
        return ESP_ERR_INVALID_STATE;
    }

    // 分块分配局部传输缓冲区（20 行高以减少内存开销）
    #define CLEAR_BUFFER_LINES 20
    uint16_t *buffer = malloc(LCD_H_RES * CLEAR_BUFFER_LINES * sizeof(uint16_t));
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "申请清屏缓存失败，内存不足");
        return ESP_ERR_NO_MEM;
    }

    // 转换成大端字节序 (SPI 液晶屏数据格式)
    uint16_t swapped_color = (color >> 8) | (color << 8);
    for (int i = 0; i < LCD_H_RES * CLEAR_BUFFER_LINES; i++)
    {
        buffer[i] = swapped_color;
    }

    esp_err_t err = ESP_OK;
    for (int y = 0; y < LCD_V_RES; y += CLEAR_BUFFER_LINES)
    {
        err = esp_lcd_panel_draw_bitmap(s_panel_handle, 0, y, LCD_H_RES, y + CLEAR_BUFFER_LINES, buffer);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "绘制清屏色块失败，在行 %d 处报错: %s", y, esp_err_to_name(err));
            break;
        }
    }

    free(buffer);
    return err;
}

/**
 * @brief 绘制位图到屏幕指定区域
 */
esp_err_t bsp_lcd_draw_bitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data)
{
    if (!s_lcd_initialized)
    {
        return ESP_ERR_INVALID_STATE;
    }
    return esp_lcd_panel_draw_bitmap(s_panel_handle, x_start, y_start, x_end, y_end, color_data);
}
