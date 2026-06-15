/**
 * @file bsp_key.c
 * @brief 板载按键驱动实现源文件，基于 iot_button 完成消抖与事件发布
 */

#include "bsp_key.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_button.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_event.h"

static const char *TAG = "bsp_key";

// 定义按键事件基 (EVENT BASE)
ESP_EVENT_DEFINE_BASE(BSP_KEY_EVENT_BASE);

static button_handle_t s_btn_handle = NULL;

/* ================================================================
 * 内部辅助函数
 * ================================================================ */

/**
 * @brief iot_button 触发的事件回调函数，负责将事件打包并发布至系统事件环
 */
static void button_event_handler_cb(void *button_handle, void *usr_data)
{
    int32_t event_id = (int32_t)(intptr_t)usr_data;
    esp_err_t err = esp_event_post(BSP_KEY_EVENT_BASE, event_id, NULL, 0, pdMS_TO_TICKS(100));
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to post key event %d: %s", (int)event_id, esp_err_to_name(err));
    }
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化板载按键并绑定系统默认事件发布逻辑
 */
void bsp_key_init(void)
{
    // 首先反初始化 GPIO0 的 RTC IO 状态，确保其重新路由回普通数字 IO MUX（防止休眠唤醒后按键失效）
    if (rtc_gpio_is_valid_gpio(0)) {
        rtc_gpio_hold_dis(0);
        rtc_gpio_deinit(0);
    }

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
    
    // 注册消抖后的单击和双击回调，事件发生时发送至系统事件总线
    esp_err_t err;
    err = iot_button_register_cb(s_btn_handle, BUTTON_SINGLE_CLICK, button_event_handler_cb, (void *)BSP_KEY_EVENT_SINGLE_CLICK);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register single click callback: %s", esp_err_to_name(err));
    }
    
    err = iot_button_register_cb(s_btn_handle, BUTTON_DOUBLE_CLICK, button_event_handler_cb, (void *)BSP_KEY_EVENT_DOUBLE_CLICK);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register double click callback: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "BOOT key initialized and registered to event loop successfully");
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
 * @brief 反初始化板载按键，释放按键设备资源
 */
void bsp_key_deinit(void)
{
    if (s_btn_handle == NULL)
    {
        ESP_LOGW(TAG, "BOOT key not initialized");
        return;
    }

    ESP_LOGI(TAG, "Deinitializing BOOT key...");

    // iot_button_delete 会释放按键设备资源
    esp_err_t err = iot_button_delete(s_btn_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete BOOT key: %s", esp_err_to_name(err));
    }

    s_btn_handle = NULL;
    ESP_LOGI(TAG, "BOOT key deinitialized successfully");
}
