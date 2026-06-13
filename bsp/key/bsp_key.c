/**
 * @file bsp_key.c
 * @brief 板载按键驱动实现源文件，基于 iot_button 完成消抖与事件驱动回调
 */

#include "bsp_key.h"
#include "iot_button.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "bsp_key";

static button_handle_t s_btn_handle = NULL;

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化板载按键
 */
void bsp_key_init(void)
{
    if (s_btn_handle != NULL)
    {
        ESP_LOGW(TAG, "BOOT key already initialized");
        return;
    }

    const button_config_t btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = 0,               // BOOT按键连至 GPIO0
            .active_level = 0,          // 按下接地为低电平 (0)
        },
    };
    
    // 初始化并创建 GPIO 按键设备
    s_btn_handle = iot_button_create(&btn_cfg);
    if (s_btn_handle == NULL)
    {
        ESP_LOGE(TAG, "Failed to create BOOT key");
        return;
    }
    
    ESP_LOGI(TAG, "BOOT key initialized successfully");
}

/**
 * @brief 读取按键实时状态
 */
bool bsp_key_is_pressed(void)
{
    // GPIO0 按下时为低电平 (0)，故当电平为 0 时返回 true
    return gpio_get_level(0) == 0;
}

/**
 * @brief 注册按键事件回调函数
 */
esp_err_t bsp_key_register_callback(bsp_key_event_t event, bsp_key_cb_t cb, void *usr_data)
{
    if (s_btn_handle == NULL)
    {
        ESP_LOGE(TAG, "Key driver not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    button_event_t btn_event;
    switch (event)
    {
        case BSP_KEY_SINGLE_CLICK:
            btn_event = BUTTON_SINGLE_CLICK;
            break;
        case BSP_KEY_DOUBLE_CLICK:
            btn_event = BUTTON_DOUBLE_CLICK;
            break;
        default:
            ESP_LOGE(TAG, "Unsupported key event: %d", event);
            return ESP_ERR_INVALID_ARG;
    }

    return iot_button_register_cb(s_btn_handle, btn_event, (button_cb_t)cb, usr_data);
}
