#include "app_init.h"
#include "app_cli.h"
#include "bsp_backlight.h"
#include "bsp_key.h"
#include "esp_log.h"

static const char *TAG = "app_init";

/* 按键单击事件回调：开启背光 */
static void app_key_single_click_handler(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BOOT Key Single Click -> Turn Backlight ON (Decoupled)");
    bsp_backlight_set(true);
}

/* 按键双击事件回调：关闭背光 */
static void app_key_double_click_handler(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BOOT Key Double Click -> Turn Backlight OFF (Decoupled)");
    bsp_backlight_set(false);
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 系统应用初始化入口函数
 */
void app_init(void)
{
    // 初始化板载背光硬件
    ESP_ERROR_CHECK(bsp_backlight_init());

    // 初始化板载按键硬件
    bsp_key_init();

    // 注册按键事件回调，绑定背光控制逻辑（解耦设计实现）
    ESP_ERROR_CHECK(bsp_key_register_callback(BSP_KEY_SINGLE_CLICK, app_key_single_click_handler, NULL));
    ESP_ERROR_CHECK(bsp_key_register_callback(BSP_KEY_DOUBLE_CLICK, app_key_double_click_handler, NULL));

    // 启动调试控制台及应用层命令行接口
    ESP_ERROR_CHECK(app_cli_init());
}
