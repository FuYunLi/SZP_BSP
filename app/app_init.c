#include "app_init.h"
#include "app_cli.h"
#include "bsp_backlight.h"
#include "bsp_key.h"
#include "bsp_i2c.h"
#include "bsp_littlefs.h"
#include "bsp_sdcard.h"
#include "qmi8658.h"
#include "pca9557.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

static const char *TAG = "app_init";

/* 统一的按键事件总线监听器，处理单击和双击事件以控制背光 */
static void app_key_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *data)
{
    if (base == BSP_KEY_EVENT_BASE)
    {
        switch (id)
        {
            case BSP_KEY_EVENT_SINGLE_CLICK:
                ESP_LOGI(TAG, "BOOT Key Single Click Event -> Turn Backlight ON (Publish-Subscribe)");
                bsp_backlight_set(true);
                break;
            case BSP_KEY_EVENT_DOUBLE_CLICK:
                ESP_LOGI(TAG, "BOOT Key Double Click Event -> Turn Backlight OFF (Publish-Subscribe)");
                bsp_backlight_set(false);
                break;
            default:
                break;
        }
    }
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 系统应用初始化入口函数
 */
void app_init(void)
{
    // 0. 初始化板载非易失性存储服务 (NVS)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS partition needs to be erased and re-initialized");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 1. 初始化系统默认事件总线
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE(TAG, "Failed to create default event loop: %s", esp_err_to_name(err));
    }

    // 1.5 挂载片上 Flash 局部文件系统 (LittleFS)
    ESP_ERROR_CHECK(bsp_littlefs_mount());

    // 1.6 挂载外部 MicroSD 卡 (SDIO 1-bit)
    // 注意：SD卡为可拔插介质，初始化失败时不能挂死系统，只记录Warning日志
    ret = bsp_sdcard_mount();
    if (ret != ESP_OK)
    {
        ESP_LOGW(TAG, "MicroSD card mount failed or not inserted (%s)", esp_err_to_name(ret));
    }

    // 2. 初始化板载背光硬件
    ESP_ERROR_CHECK(bsp_backlight_init());

    // 3. 初始化板载按键硬件并绑定事件发布逻辑
    bsp_key_init();

    // 4. 初始化板载 I2C1 主总线
    ESP_ERROR_CHECK(bsp_i2c_init());

    // 5. 初始化板载 IO 扩展芯片 PCA9557
    ESP_ERROR_CHECK(pca9557_init(bsp_i2c_get_bus_handle()));

    // 5.5 初始化板载 6 轴姿态传感器 QMI8658A
    ret = qmi8658_init(bsp_i2c_get_bus_handle());
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize QMI8658A IMU (%s)", esp_err_to_name(ret));
    }

    // 6. 注册按键事件监听器，订阅按键事件并绑定背光联动逻辑
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        BSP_KEY_EVENT_BASE,
        ESP_EVENT_ANY_ID,
        app_key_event_handler,
        NULL,
        NULL
    ));

    // 7. 启动调试控制台及应用层命令行接口
    ESP_ERROR_CHECK(app_cli_init());
}
