#include "bsp_audio.h"
#include "pca9557.h"
#include "bsp_i2c.h"
#include "esp_log.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "es8311.h"
#include "es7210.h"

static const char *TAG = "bsp_audio";

static i2s_chan_handle_t s_tx_handle = NULL;
static i2s_chan_handle_t s_rx_handle = NULL;
static es8311_handle_t s_codec_handle = NULL;
static es7210_handle_t s_adc_handle = NULL;
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

    // 1. 分配 I2S0 控制器的 TX 和 RX 通道（Master模式），并精简 DMA 参数以节约内部 SRAM
    i2s_chan_config_t chan_cfg = {
        .id = I2S_NUM_0,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = 4,      // 从默认 of 6 down to 4 to save descriptors
        .dma_frame_num = 128,   // 从默认 of 240 down to 128 to save buffer SRAM
        .auto_clear = true,
    };
    esp_err_t ret = i2s_new_channel(&chan_cfg, &s_tx_handle, &s_rx_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "分配 I2S TX/RX 通道失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 2. 配置 I2S0 STD 模式参数 (MCK->IO38, BCK->IO14, WS->IO13, DO->IO45, DI->IO12)
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000), // 默认使用 16kHz 采样率，后期播放可动态重构
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = 38,
            .bclk = 14,
            .ws   = 13,
            .dout = 45,
            .din  = 12, // 连接音频 ADC (ES7210 SDOUT)
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    // 3. 初始化 TX & RX 为标准模式并开启 I2S 时钟输出
    if (s_tx_handle)
    {
        ret = i2s_channel_init_std_mode(s_tx_handle, &std_cfg);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "初始化 I2S TX 通道失败: %s", esp_err_to_name(ret));
            goto err_free_channels;
        }
        ESP_LOGI(TAG, "I2S TX 通道初始化 STD 模式成功");

        ret = i2s_channel_enable(s_tx_handle);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "启用 I2S TX 通道失败: %s", esp_err_to_name(ret));
            goto err_free_channels;
        }
        ESP_LOGI(TAG, "I2S TX 通道启用成功，时钟信号已输出");
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

        ret = i2s_channel_enable(s_rx_handle);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "启用 I2S RX 通道失败: %s", esp_err_to_name(ret));
            goto err_free_channels;
        }
        ESP_LOGI(TAG, "I2S RX 通道启用成功，录音接口已准备就绪");
    }

    // 4. 配置并初始化音频 Codec (ES8311)
    ESP_LOGI(TAG, "正在通过 I2C 初始化音频 Codec ES8311...");
    s_codec_handle = es8311_create(bsp_i2c_get_bus_handle(), ES8311_ADDRESS_0);
    if (s_codec_handle == NULL)
    {
        ESP_LOGE(TAG, "创建 ES8311 实例句柄失败");
        ret = ESP_FAIL;
        goto err_free_channels;
    }

    es8311_clock_config_t clk_cfg = {
        .mclk_inverted = false,
        .sclk_inverted = false,
        .mclk_from_mclk_pin = true,
        .mclk_frequency = 16000 * 256, // 16kHz * 256 = 4096000 Hz
        .sample_frequency = 16000,     // 16kHz
    };

    ret = es8311_init(s_codec_handle, &clk_cfg, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 ES8311 寄存器失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }

    // 设置初始音量为 80%
    int volume_set = 80;
    ret = es8311_voice_volume_set(s_codec_handle, 80, &volume_set);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "配置 ES8311 音量失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }
    ESP_LOGI(TAG, "音频 Codec ES8311 配置音量为 80 完成");

    // 5. 配置并初始化音频录音 ADC (ES7210)
    // 注意：ES7210 极其依赖时钟输入，必须在 I2S 通道拉起（MCLK 输出）后方可由 I2C 初始化
    ESP_LOGI(TAG, "正在通过 I2C 初始化录音芯片 ES7210...");
    s_adc_handle = es7210_create(bsp_i2c_get_bus_handle(), ES7210_AD0_AD1_01_ADDR);
    if (s_adc_handle == NULL)
    {
        ESP_LOGE(TAG, "创建 ES7210 实例句柄失败");
        ret = ESP_FAIL;
        goto err_free_channels;
    }

    es7210_config_t adc_cfg = {
        .mic_selected = ES7210_INPUT_MIC1 | ES7210_INPUT_MIC2 | ES7210_INPUT_MIC3, // 开启板载三麦克风
        .gain_db = 30.0,
        .master_mode = false,
    };

    ret = es7210_init(s_adc_handle, &adc_cfg);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 ES7210 寄存器失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }
    ESP_LOGI(TAG, "音频录音芯片 ES7210 初始化配置成功");

    // 6. 配置 PCA9557 Pin 1 (PA_EN) 为输出模式
    ESP_LOGI(TAG, "配置 PCA9557 扩展芯片 Pin 1 为功放使能输出...");
    ret = pca9557_set_config(1, false);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "配置 PCA9557 Pin 1 为输出失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }

    // 7. 时序消噪控制：在 Codec 内部电平稳定后（延迟 100ms）再拉高功放使能脚 PA_EN
    vTaskDelay(pdMS_TO_TICKS(100));
    ret = pca9557_set_output_level(1, 1);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "拉高 PCA9557 Pin 1 (PA_EN) 失败: %s", esp_err_to_name(ret));
        goto err_free_channels;
    }
    ESP_LOGI(TAG, "已使能功放功耗输入 (PA_EN = 1)");

    s_audio_initialized = true;
    ESP_LOGI(TAG, "板载 I2S0 与音频功放/ADC 硬件初始化成功完成");
    return ESP_OK;

err_free_channels:
    if (s_adc_handle)
    {
        es7210_delete(s_adc_handle);
        s_adc_handle = NULL;
    }
    if (s_codec_handle)
    {
        es8311_delete(s_codec_handle);
        s_codec_handle = NULL;
    }
    if (s_rx_handle)
    {
        i2s_channel_disable(s_rx_handle);
        i2s_del_channel(s_rx_handle);
        s_rx_handle = NULL;
    }
    if (s_tx_handle)
    {
        i2s_channel_disable(s_tx_handle);
        i2s_del_channel(s_tx_handle);
        s_tx_handle = NULL;
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

/**
 * @brief 设置板载音频 Codec 输出音量
 */
esp_err_t bsp_audio_volume_set(uint8_t volume)
{
    if (s_codec_handle == NULL)
    {
        ESP_LOGW(TAG, "Codec 未初始化，无法设置音量");
        return ESP_ERR_INVALID_STATE;
    }
    int volume_set = (int)volume;
    esp_err_t ret = es8311_voice_volume_set(s_codec_handle, (int)volume, &volume_set);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "设置音量失败: %s", esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t bsp_audio_record_read(void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms)
{
    if (s_rx_handle == NULL)
    {
        ESP_LOGE(TAG, "I2S RX 通道未初始化");
        return ESP_ERR_INVALID_STATE;
    }
    return i2s_channel_read(s_rx_handle, dest, size, bytes_read, pdMS_TO_TICKS(timeout_ms));
}
