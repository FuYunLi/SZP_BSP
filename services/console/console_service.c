#include "console_service.h"
#include "esp_console.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/uart.h"

static const char *TAG = "console_svc";

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

    // 启动 REPL 交互环境
    err = esp_console_start_repl(repl);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start REPL: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Console service started.");
    return ESP_OK;
}
