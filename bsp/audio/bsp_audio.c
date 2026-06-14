/**
 * @file bsp_audio.c
 * @brief 板载 I2S 音频接口与功放控制板级支持包实现源文件
 */

#include "bsp_audio.h"
#include "pca9557.h"
#include "esp_log.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "bsp_audio";

static i2s_chan_handle_t s_tx_handle = NULL;
static i2s_chan_handle_t s_rx_handle = NULL;
static bool s_audio_initialized = false;

/**
 * @brief 初始化板级音频 I2S0 标准通道及配置功放引脚
 */
esp_err_t bsp_audio_i2s_init(void)
{
    if (s_audio_initialized)
    {
        ESP_LOGW(TAG, "音频驱动已初始化过");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "正在初始化 I2S0 标准通道 (TX/RX)...");

    // 1. 分配 I2S0 控制器的 TX 和 RX 通道（Master模式）
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    esp_err_t ret = i2s_new_channel(&chan_cfg, &s_tx_handle, &s_rx_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "分配 I2S 通道失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 2. 配置 I2S0 STD 模式参数
    // MCK->IO38, BCK->IO14, WS->IO13, DO->IO45, DI->IO12
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000), // 默认使用 16kHz 采样率，后期播放/录音可动态重构
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = 38,
            .bclk = 14,
            .ws   = 13,
            .dout = 45,
            .din  = 12,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    // 3. 分别初始化 TX 和 RX 为标准模式
    if (s_tx_handle)
    {
        ret = i2s_channel_init_std_mode(s_tx_handle, &std_cfg);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "初始化 I2S TX 通道失败: %s", esp_err_to_name(ret));
            goto err_free_channels;
        }
        ESP_LOGI(TAG, "I2S TX 通道初始化 STD 模式成功");
    }

    if (s_rx_handle)
    {
        ret = i2s_channel_init_std_mode(s_rx_handle, &std_cfg);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "初始化 I2S RX 通道失败: %s", esp_err_to_name(ret));
            goto err_free_channels;
        }
        ESP_LOGI(TAG, "I2S RX 通道初始化 STD 模式成功");
    }

    // 4. 配置 PCA9557 Pin 1 (PA_EN) 为输出模式
    ESP_LOGI(TAG, "配置 PCA9557 扩展芯片 Pin 1 为功放使能输出...");
    ret = pca9557_set_config(1, false);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "配置 PCA9557 Pin 1 为输出失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }

    // 5. 按照 M16 任务要求，调用 bsp_pca9557 将音频功放使能脚 PA_EN 置为高电平
    // 为了防止突发杂音，我们在使能功放前添加微小延时
    vTaskDelay(pdMS_TO_TICKS(50));
    ret = pca9557_set_output_level(1, 1);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "拉高 PCA9557 Pin 1 (PA_EN) 失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }
    ESP_LOGI(TAG, "已使能功放功耗输入 (PA_EN = 1)");

    s_audio_initialized = true;
    ESP_LOGI(TAG, "板载 I2S0 与音频功放硬件初始化成功完成");
    return ESP_OK;

err_free_channels:
    if (s_tx_handle || s_rx_handle)
    {
        i2s_del_channel(s_tx_handle);
        i2s_del_channel(s_rx_handle);
        s_tx_handle = NULL;
        s_rx_handle = NULL;
    }
    return ret;
}

/**
 * @brief 板级音频功放使能控制
 */
esp_err_t bsp_audio_pa_enable(bool enable)
{
    ESP_LOGI(TAG, "%s 音频功放使能脚 PA_EN...", enable ? "开启" : "关闭");
    esp_err_t ret = pca9557_set_output_level(1, enable ? 1 : 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "设置功放使能脚状态失败: %s", esp_err_to_name(ret));
    }
    return ret;
}

/**
 * @brief 获取已注册的 I2S TX 通道句柄
 */
i2s_chan_handle_t bsp_audio_get_tx_handle(void)
{
    return s_tx_handle;
}

/**
 * @brief 获取已注册的 I2S RX 通道句柄
 */
i2s_chan_handle_t bsp_audio_get_rx_handle(void)
{
    return s_rx_handle;
}
