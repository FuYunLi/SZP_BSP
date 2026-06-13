#include "bsp_i2c.h"
#include "esp_log.h"

static const char *TAG = "bsp_i2c";
static i2c_master_bus_handle_t s_i2c_bus_handle = NULL;

esp_err_t bsp_i2c_init(void)
{
    if (s_i2c_bus_handle != NULL)
    {
        ESP_LOGW(TAG, "I2C1 bus already initialized");
        return ESP_OK;
    }

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_1,
        .sda_io_num = BSP_I2C_SDA_PIN,
        .scl_io_num = BSP_I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t err = i2c_new_master_bus(&bus_config, &s_i2c_bus_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize I2C1 bus: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "I2C1 bus initialized successfully (SDA: GPIO%d, SCL: GPIO%d, Freq: %dHz)", 
             BSP_I2C_SDA_PIN, BSP_I2C_SCL_PIN, BSP_I2C_FREQ_HZ);
    return ESP_OK;
}

i2c_master_bus_handle_t bsp_i2c_get_bus_handle(void)
{
    return s_i2c_bus_handle;
}

esp_err_t bsp_i2c_deinit(void)
{
    if (s_i2c_bus_handle == NULL)
    {
        ESP_LOGW(TAG, "I2C1 bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err = i2c_del_master_bus(s_i2c_bus_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete I2C1 bus: %s", esp_err_to_name(err));
        return err;
    }

    s_i2c_bus_handle = NULL;
    ESP_LOGI(TAG, "I2C1 bus de-initialized successfully");
    return ESP_OK;
}
