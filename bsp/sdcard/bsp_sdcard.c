/**
 * @file bsp_sdcard.c
 * @brief 板级支持包外部 MicroSD 卡接口实现
 */

#include "bsp_sdcard.h"
#include "esp_log.h"
#include "driver/sdmmc_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"

static const char *TAG = "bsp_sdcard";
static bool s_is_mounted = false;
static sdmmc_card_t *s_card = NULL;

#define SD_CARD_BASE_PATH "/sdcard"

// Pins definitions
#define SD_CARD_PIN_CLK  GPIO_NUM_47
#define SD_CARD_PIN_CMD  GPIO_NUM_48
#define SD_CARD_PIN_D0   GPIO_NUM_21

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 挂载外部 MicroSD 卡到 /sdcard
 */
esp_err_t bsp_sdcard_mount(void)
{
    if (s_is_mounted)
    {
        ESP_LOGW(TAG, "SD card already mounted");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Mounting MicroSD card (1-bit SDIO)...");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    // Configure to 1-bit width
    host.flags = SDMMC_HOST_FLAG_1BIT;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT; // 20 MHz, safer for signal integrity

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.clk = SD_CARD_PIN_CLK;
    slot_config.cmd = SD_CARD_PIN_CMD;
    slot_config.d0 = SD_CARD_PIN_D0;
    slot_config.width = 1;
    // Enable internal pullups on enabled pins
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    esp_err_t ret = esp_vfs_fat_sdmmc_mount(SD_CARD_BASE_PATH, &host, &slot_config, &mount_config, &s_card);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount SD card (%s)", esp_err_to_name(ret));
        s_card = NULL;
        return ret;
    }

    s_is_mounted = true;
    ESP_LOGI(TAG, "SD card mounted successfully at '%s'", SD_CARD_BASE_PATH);
    
    // Print card properties
    sdmmc_card_print_info(stdout, s_card);

    return ESP_OK;
}

/**
 * @brief 卸载 MicroSD 卡
 */
esp_err_t bsp_sdcard_unmount(void)
{
    if (!s_is_mounted)
    {
        ESP_LOGW(TAG, "SD card not mounted");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = esp_vfs_fat_sdcard_unmount(SD_CARD_BASE_PATH, s_card);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to unmount SD card (%s)", esp_err_to_name(ret));
        return ret;
    }

    s_is_mounted = false;
    s_card = NULL;
    ESP_LOGI(TAG, "SD card unmounted successfully");

    return ESP_OK;
}

/**
 * @brief 检查 MicroSD 卡是否已挂载
 */
bool bsp_sdcard_is_mounted(void)
{
    return s_is_mounted;
}
