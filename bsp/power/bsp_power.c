/**
 * @file bsp_power.c
 * @brief 板级电源与低功耗管理系统实现
 * @version 1.1
 * @date 2026-06-13
 */

#include "bsp_power.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "bsp_power";

#define WAKEUP_GPIO_PIN     GPIO_NUM_0

esp_err_t bsp_power_init(void)
{
    ESP_LOGI(TAG, "正在初始化板级电源管理...");
#if CONFIG_PM_ENABLE
    esp_pm_config_t pm_config = {
        .max_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = rtc_clk_xtal_freq_get(), // 自适应获取外部晶振频率 (如 ESP32-S3 为 40MHz)
#if CONFIG_FREERTOS_USE_TICKLESS_IDLE
        .light_sleep_enable = true
#else
        .light_sleep_enable = false
#endif
    };
    esp_err_t err = esp_pm_configure(&pm_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "配置电源管理失败: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "DFS 与自动轻度睡眠配置成功 (最高频率: %d MHz, 最低频率: %d MHz, 自动休眠: %s)",
             (int)pm_config.max_freq_mhz, (int)pm_config.min_freq_mhz,
             pm_config.light_sleep_enable ? "已启用" : "已禁用");
    return ESP_OK;
#else
    ESP_LOGW(TAG, "sdkconfig 中未启用电源管理 (CONFIG_PM_ENABLE)");
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t bsp_power_enter_sleep(bsp_sleep_mode_t mode)
{
    esp_err_t err = ESP_OK;

    if (mode == BSP_SLEEP_MODE_DEEP) {
        // 1. 配置 BOOT 按键 (GPIO0) 作为 RTC 中断唤醒源
        // 当按键按下时，GPIO0 呈现低电平 (0) 触发唤醒
        err = esp_sleep_enable_ext1_wakeup(1ULL << WAKEUP_GPIO_PIN, ESP_EXT1_WAKEUP_ANY_LOW);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "启用 GPIO%d 上的 EXT1 唤醒源失败: %s", WAKEUP_GPIO_PIN, esp_err_to_name(err));
            return err;
        }

        // 2. 启用内部上拉并禁用下拉，确保休眠时 GPIO0 电平稳定
        rtc_gpio_pullup_en(WAKEUP_GPIO_PIN);
        rtc_gpio_pulldown_dis(WAKEUP_GPIO_PIN);

        ESP_LOGI(TAG, "正在进入深度睡眠 (Deep-sleep)...");
        
        // 进入深度睡眠（此调用不返回，唤醒时系统复位重启）
        esp_deep_sleep_start();
        return ESP_OK;
    } else if (mode == BSP_SLEEP_MODE_LIGHT) {
#if CONFIG_PM_ENABLE
        esp_pm_config_t pm_config = {0};
        bool original_light_sleep_state = false;
        
        // 1. 获取当前 PM 配置，保存原有自动休眠状态
        err = esp_pm_get_configuration(&pm_config);
        if (err == ESP_OK) {
            original_light_sleep_state = pm_config.light_sleep_enable;
            
            // 临时关闭自动轻度睡眠，防止干扰手动休眠
            pm_config.light_sleep_enable = false;
            esp_pm_configure(&pm_config);
        } else {
            ESP_LOGW(TAG, "获取电源管理配置失败: %s，将使用默认安全模式", esp_err_to_name(err));
        }
#endif

        // 2. 配置 BOOT 按键 (GPIO0) 作为普通 GPIO 输入并开启上拉
        gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << WAKEUP_GPIO_PIN,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_LOW_LEVEL
        };
        gpio_config(&io_conf);

        // 3. 启用引脚级别的低电平唤醒
        err = gpio_wakeup_enable(WAKEUP_GPIO_PIN, GPIO_INTR_LOW_LEVEL);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "在 GPIO%d 上启用 GPIO 唤醒失败: %s", WAKEUP_GPIO_PIN, esp_err_to_name(err));
            goto light_sleep_cleanup;
        }

        // 4. 开启全局 GPIO 唤醒源
        err = esp_sleep_enable_gpio_wakeup();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "开启全局 GPIO 休眠唤醒源失败: %s", esp_err_to_name(err));
            gpio_wakeup_disable(WAKEUP_GPIO_PIN);
            goto light_sleep_cleanup;
        }

        ESP_LOGI(TAG, "正在进入轻度睡眠 (Light-sleep)...");
        
        // 进入轻度睡眠并等待唤醒
        esp_light_sleep_start();
        
        // 5. 打印唤醒原因
        esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
        ESP_LOGI(TAG, "从轻度睡眠唤醒! 唤醒源代号: %d", cause);
        
        // 6. 唤醒后：关闭该引脚的唤醒配置，防止影响常规工作
        gpio_wakeup_disable(WAKEUP_GPIO_PIN);

light_sleep_cleanup:
        // 7. 安全恢复 PM 配置的原始状态
#if CONFIG_PM_ENABLE
        if (esp_pm_get_configuration(&pm_config) == ESP_OK) {
            pm_config.light_sleep_enable = original_light_sleep_state;
            esp_pm_configure(&pm_config);
        }
#endif
        return err;
    }

    return ESP_ERR_INVALID_ARG;
}
