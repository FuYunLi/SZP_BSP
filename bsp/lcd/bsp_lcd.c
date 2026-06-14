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
#include "esp_heap_caps.h"
#include <stdlib.h>

#define LCD_HOST               SPI2_HOST
#define PIN_NUM_LCD_DC         39
#define PIN_NUM_LCD_MOSI       40
#define PIN_NUM_LCD_CLK        41
#define CLEAR_BUFFER_LINES     20

static const char *TAG = "bsp_lcd";
static esp_lcd_panel_handle_t s_panel_handle = NULL;
static esp_lcd_panel_io_handle_t s_io_handle = NULL;
static uint16_t *s_clear_buffer = NULL; // 静态分配的清屏/局部刷新缓冲区，规避系统频繁 malloc/free 导致堆内存碎片化
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
        .max_transfer_sz = LCD_H_RES * CLEAR_BUFFER_LINES * sizeof(uint16_t), // 设置对应传输限额的缓冲区大小
    };

    esp_err_t ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 SPI 总线失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在通过 PCA9557 配置 LCD_CS 引脚为输出...");
    // 配置 PCA9557 Pin 0 (LCD_CS) 为输出，复位时先保持高电平以防止 SPI 初始化期间毛刺影响 ST7789
    ret = pca9557_set_config(0, false);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "配置 PCA9557 扩展引脚为输出失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在创建 LCD 面板 IO 句柄...");
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_LCD_DC,
        .cs_gpio_num = -1, // 片选由外部 PCA9557 手动管理，此处传入 -1
        .pclk_hz = 80 * 1000 * 1000, // 80MHz 像素时钟 (与官方例程一致)
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 2, // 使用 SPI Mode 2 (与官方例程一致)
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
        .reset_gpio_num = -1, // 硬件复位直接连至系统 CHIP_PU，配置为 -1
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE, // 启用 ST7789 硬件小端字节序解析，避免 CPU 软交换开销
        .bits_per_pixel = 16,
    };

    ret = esp_lcd_new_panel_st7789(s_io_handle, &panel_config, &s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "加载 ST7789 面板驱动失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在对 LCD 面板进行软复位...");
    ret = esp_lcd_panel_reset(s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "重置 LCD 面板失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "拉低 LCD_CS 使能通信...");
    // 复位完成后拉低片选信号，确保后续的寄存器配置在干净的总线状态下发送
    ret = pca9557_set_output_level(0, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "拉低 LCD_CS 失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "正在执行 LCD 面板初始化寄存器配置...");
    ret = esp_lcd_panel_init(s_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 LCD 面板失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 屏幕显示基本配置：开启显示、开启反色、坐标轴调换、X轴/Y轴镜像配置
    esp_lcd_panel_disp_on_off(s_panel_handle, true);
    esp_lcd_panel_invert_color(s_panel_handle, true);
    esp_lcd_panel_swap_xy(s_panel_handle, true);
    esp_lcd_panel_mirror(s_panel_handle, true, false); // 镜像配置 (x=true, y=false)

    // 静态分配清屏缓冲区，规避系统长期运行频繁 malloc/free 导致堆内存碎片化
    s_clear_buffer = heap_caps_malloc(LCD_H_RES * CLEAR_BUFFER_LINES * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    if (s_clear_buffer == NULL)
    {
        ESP_LOGE(TAG, "申请静态清屏缓存失败，内存不足");
        esp_lcd_panel_del(s_panel_handle);
        esp_lcd_panel_io_del(s_io_handle);
        spi_bus_free(LCD_HOST);
        s_panel_handle = NULL;
        s_io_handle = NULL;
        return ESP_ERR_NO_MEM;
    }

    s_lcd_initialized = true;
    ESP_LOGI(TAG, "LCD 驱动配置及初始化成功完成");
    return ESP_OK;
}

/**
 * @brief 填充整个 LCD 屏幕为指定颜色 (清屏)
 */
esp_err_t bsp_lcd_clear(uint16_t color)
{
    if (!s_lcd_initialized || s_clear_buffer == NULL)
    {
        ESP_LOGE(TAG, "LCD 未初始化或缓冲区无效，无法执行清屏");
        return ESP_ERR_INVALID_STATE;
    }

    // 直接使用主机原生小端字节序填充，由 ST7789 硬件控制器在接收时自动完成字节序解析，节省 CPU 软交换开销
    for (int i = 0; i < LCD_H_RES * CLEAR_BUFFER_LINES; i++)
    {
        s_clear_buffer[i] = color;
    }

    esp_err_t err = ESP_OK;
    for (int y = 0; y < LCD_V_RES; y += CLEAR_BUFFER_LINES)
    {
        err = esp_lcd_panel_draw_bitmap(s_panel_handle, 0, y, LCD_H_RES, y + CLEAR_BUFFER_LINES, s_clear_buffer);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "绘制清屏色块失败，在行 %d 处报错: %s", y, esp_err_to_name(err));
            break;
        }
    }

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

/**
 * @brief 注册 LCD SPI 传输完成中断回调 (面向 LVGL 异步刷新)
 */
esp_err_t bsp_lcd_register_trans_done_cb(esp_lcd_panel_io_color_trans_done_cb_t cb, void *user_ctx)
{
    if (!s_lcd_initialized || s_io_handle == NULL)
    {
        ESP_LOGE(TAG, "LCD IO 未初始化，无法注册回调");
        return ESP_ERR_INVALID_STATE;
    }

    esp_lcd_panel_io_callbacks_t cbs = {
        .on_color_trans_done = cb,
    };
    return esp_lcd_panel_io_register_event_callbacks(s_io_handle, &cbs, user_ctx);
}

/**
 * @brief 获取 LCD 面板驱动句柄
 */
esp_lcd_panel_handle_t bsp_lcd_get_panel_handle(void)
{
    return s_panel_handle;
}

/**
 * @brief 获取 LCD IO 驱动句柄
 */
esp_lcd_panel_io_handle_t bsp_lcd_get_io_handle(void)
{
    return s_io_handle;
}
