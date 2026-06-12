/**
 * @file app_cli.c
 * @brief 应用层命令行接口实现，用于集中注册与管理当前项目的诊断、控制及测试命令
 */

#include "app_cli.h"
#include "console_service.h"
#include "esp_console.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

static const char *TAG = "app_cli";

/* 版本查询命令的回调函数 */
static int do_version_cmd(int argc, char **argv)
{
    printf("立创实战派 S3 BSP v1.0.0\n");
    return 0;
}

/* 注册系统级的通用诊断指令 */
static void register_system_commands(void)
{
    const esp_console_cmd_t cmd = {
        .command = "version",
        .help = "Get system/BSP version information",
        .hint = NULL,
        .func = &do_version_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化并注册应用层的控制台命令
 */
esp_err_t app_cli_init(void)
{
    ESP_ERROR_CHECK(console_service_start());
    
    // 注册自定义的命令
    register_system_commands();
    
    return ESP_OK;
}