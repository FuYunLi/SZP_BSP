/**
 * @file bsp_backlight.c
 * @brief 屏幕背光板级支持包源文件，实现 GPIO 极简开关控制
 */

#include "bsp_backlight.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BSP_LCD_BL_GPIO     42
#define BSP_LCD_BL_ON       0    // PMOS 低电平导通点亮
#define BSP_LCD_BL_OFF      1    // PMOS 高电平截止熄灭

static const char *TAG = "bsp_backlight";

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化板载屏幕背光引脚为输出模式
 */
esp_err_t bsp_backlight_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BSP_LCD_BL_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    // 初始化 GPIO 配置
    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure Backlight GPIO: %s", esp_err_to_name(err));
        return err;
    }
    
    // 默认关闭背光以节省能耗
    return bsp_backlight_set(false);
}

/**
 * @brief 设置板载屏幕背光状态
 */
esp_err_t bsp_backlight_set(bool enable)
{
    int level = enable ? BSP_LCD_BL_ON : BSP_LCD_BL_OFF;
    
    esp_err_t err = gpio_set_level(BSP_LCD_BL_GPIO, level);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set Backlight GPIO level: %s", esp_err_to_name(err));
        return err;
    }
    
    return ESP_OK;
}

/**
 * @brief 反初始化板载屏幕背光引脚
 */
esp_err_t bsp_backlight_deinit(void)
{
    ESP_LOGI(TAG, "Deinitializing backlight GPIO...");
    
    // 先关闭背光
    esp_err_t err = bsp_backlight_set(false);
    if (err != ESP_OK)
    {
        return err;
    }
    
    // 重置 GPIO 配置为输入模式（安全状态）
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BSP_LCD_BL_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    err = gpio_config(&io_conf);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to reset Backlight GPIO: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "Backlight GPIO deinitialized successfully");
    return ESP_OK;
}
