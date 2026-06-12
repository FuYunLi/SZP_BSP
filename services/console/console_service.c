#include "console_service.h"
#include <stdio.h>
#include <string.h>
#include "esp_console.h"
#include "esp_log.h"
#include "driver/uart.h"

static const char *TAG = "console_svc";

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
 * @brief 初始化并启动控制台服务
 */
esp_err_t console_service_start(void)
{
    ESP_LOGI(TAG, "Starting console service...");

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = CONFIG_CONSOLE_PROMPT_STRING;
    repl_config.max_cmdline_length = 256;

    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    
    // 初始化并创建 UART-based REPL 实例
    esp_err_t err = esp_console_new_repl_uart(&uart_config, &repl_config, &repl);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create REPL UART: %s", esp_err_to_name(err));
        return err;
    }

    // 注册自定义的命令
    register_system_commands();

    // 启动 REPL 交互环境
    err = esp_console_start_repl(repl);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start REPL: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Console service started.");
    return ESP_OK;
}
