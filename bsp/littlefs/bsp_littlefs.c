/**
 * @file bsp_littlefs.c
 * @brief 板级支持包内部 Flash LittleFS 文件系统接口实现
 */

#include "bsp_littlefs.h"
#include "esp_log.h"
#include "esp_littlefs.h"

static const char *TAG = "bsp_littlefs";
static bool s_is_mounted = false;

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 挂载片上 Flash 的 LittleFS 分区
 */
esp_err_t bsp_littlefs_mount(void)
{
    if (s_is_mounted)
    {
        ESP_LOGW(TAG, "LittleFS partition already mounted");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Mounting LittleFS partition 'storage'...");

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "storage",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find LittleFS partition 'storage'");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }

    s_is_mounted = true;
    ESP_LOGI(TAG, "LittleFS partition 'storage' mounted successfully at '/littlefs'");

    return ESP_OK;
}

/**
 * @brief 卸载 LittleFS 分区
 */
esp_err_t bsp_littlefs_unmount(void)
{
    if (!s_is_mounted)
    {
        ESP_LOGW(TAG, "LittleFS partition not mounted");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = esp_vfs_littlefs_unregister("storage");
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to unmount LittleFS partition (%s)", esp_err_to_name(ret));
        return ret;
    }

    s_is_mounted = false;
    ESP_LOGI(TAG, "LittleFS partition 'storage' unmounted successfully");

    return ESP_OK;
}

/**
 * @brief 检查 LittleFS 是否已挂载
 */
bool bsp_littlefs_is_mounted(void)
{
    return s_is_mounted;
}
