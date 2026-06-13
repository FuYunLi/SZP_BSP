/**
 * @file app_cli.c
 * @brief 应用层命令行接口实现，用于集中注册与管理当前项目的诊断、控制及测试命令
 */

#include "app_cli.h"
#include "console_service.h"
#include "bsp_backlight.h"
#include "esp_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "bsp_key.h"

static const char *TAG = "app_cli";

/* 版本查询命令的回调函数 */
static int do_version_cmd(int argc, char **argv)
{
    printf("立创实战派 S3 BSP v1.0.0\n");
    return 0;
}

/* 屏幕背光控制命令的回调函数 */
static int do_backlight_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: backlight <0|1>\n");
        return 1;
    }
    
    int enable = atoi(argv[1]);
    if (enable != 0 && enable != 1)
    {
        printf("Invalid argument. Usage: backlight <0|1>\n");
        return 1;
    }
    
    esp_err_t err = bsp_backlight_set(enable == 1);
    if (err != ESP_OK)
    {
        printf("Failed to set backlight state: %s\n", esp_err_to_name(err));
        return 1;
    }
    
    printf("Backlight set to %s\n", enable ? "ON" : "OFF");
    return 0;
}

/* 按键状态查询命令的回调函数 */
static int do_key_cmd(int argc, char **argv)
{
    bool pressed = bsp_key_is_pressed();
    printf("BOOT Key status: %s (Level: %d)\n", pressed ? "PRESSED" : "RELEASED", gpio_get_level(0));
    return 0;
}

/* 注册系统级的通用诊断指令 */
static void register_system_commands(void)
{
    const esp_console_cmd_t version_cmd = {
        .command = "version",
        .help = "Get system/BSP version information",
        .hint = NULL,
        .func = &do_version_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&version_cmd));

    const esp_console_cmd_t backlight_cmd = {
        .command = "backlight",
        .help = "Control LCD backlight: backlight <0|1>",
        .hint = NULL,
        .func = &do_backlight_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&backlight_cmd));

    const esp_console_cmd_t key_cmd = {
        .command = "key",
        .help = "Get BOOT key level and pressed status",
        .hint = NULL,
        .func = &do_key_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&key_cmd));
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化并注册应用层的控制台命令
 */
esp_err_t app_cli_init(void)
{
    ESP_LOGI(TAG, "Initializing app CLI...");
    
    ESP_ERROR_CHECK(console_service_start());
    
    // 注册自定义的命令
    register_system_commands();
    
    return ESP_OK;
}