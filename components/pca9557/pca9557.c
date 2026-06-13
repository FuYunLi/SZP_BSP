#include "pca9557.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static const char *TAG = "pca9557";

static i2c_master_dev_handle_t s_pca9557_dev_handle = NULL;
static SemaphoreHandle_t s_pca9557_mutex = NULL;

/* 影子寄存器：在 RAM 中缓存写寄存器的状态，避免 I2C 慢速读取及提高抗干扰能力 */
static uint8_t s_output_reg_cache = 0xFD; // 默认值 REG_OUTPUT_PORT: LCD_CS=1, PA_EN=0, DVP_PWDN=1
static uint8_t s_config_reg_cache = 0xF8; // 默认值 REG_CONFIGURATION: IO0,IO1,IO2为输出

/* 芯片内部寄存器地址定义 */
#define REG_INPUT_PORT      (0x00)
#define REG_OUTPUT_PORT     (0x01)
#define REG_POLARITY_INV    (0x02)
#define REG_CONFIGURATION   (0x03)

static esp_err_t s_pca9557_write_reg(uint8_t reg_addr, uint8_t value)
{
    if (s_pca9557_dev_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }
    uint8_t data[2] = {reg_addr, value};
    return i2c_master_transmit(s_pca9557_dev_handle, data, sizeof(data), 1000);
}

static esp_err_t s_pca9557_read_reg(uint8_t reg_addr, uint8_t *value)
{
    if (s_pca9557_dev_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_transmit_receive(s_pca9557_dev_handle, &reg_addr, 1, value, 1, 1000);
}

esp_err_t pca9557_init(i2c_master_bus_handle_t bus_handle)
{
    if (s_pca9557_dev_handle != NULL)
    {
        ESP_LOGW(TAG, "PCA9557 already initialized");
        return ESP_OK;
    }

    if (bus_handle == NULL)
    {
        ESP_LOGE(TAG, "Invalid I2C bus handle");
        return ESP_ERR_INVALID_ARG;
    }

    s_pca9557_mutex = xSemaphoreCreateMutex();
    if (s_pca9557_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_ERR_NO_MEM;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = PCA9557_I2C_ADDR,
        .scl_speed_hz = 100000,
    };

    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_config, &s_pca9557_dev_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add PCA9557 to I2C bus: %s", esp_err_to_name(err));
        vSemaphoreDelete(s_pca9557_mutex);
        s_pca9557_mutex = NULL;
        return err;
    }

    // 写入默认安全电平与方向配置
    err = s_pca9557_write_reg(REG_OUTPUT_PORT, s_output_reg_cache);
    if (err == ESP_OK)
    {
        err = s_pca9557_write_reg(REG_CONFIGURATION, s_config_reg_cache);
    }

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure registers: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "PCA9557 initialized successfully (Shadow Output: 0x%02X, Config: 0x%02X)", 
             s_output_reg_cache, s_config_reg_cache);
    return ESP_OK;
}

esp_err_t pca9557_set_config(uint8_t pin, bool is_input)
{
    if (pin > 7)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_pca9557_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_pca9557_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t prev_val = s_config_reg_cache;
    if (is_input)
    {
        s_config_reg_cache |= (1 << pin);
    }
    else
    {
        s_config_reg_cache &= ~(1 << pin);
    }

    esp_err_t err = ESP_OK;
    if (s_config_reg_cache != prev_val)
    {
        err = s_pca9557_write_reg(REG_CONFIGURATION, s_config_reg_cache);
        if (err != ESP_OK)
        {
            s_config_reg_cache = prev_val; // 写入失败时还原影子寄存器
        }
    }

    xSemaphoreGive(s_pca9557_mutex);
    return err;
}

esp_err_t pca9557_set_output_level(uint8_t pin, uint8_t level)
{
    if (pin > 7)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_pca9557_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_pca9557_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t prev_val = s_output_reg_cache;
    if (level)
    {
        s_output_reg_cache |= (1 << pin);
    }
    else
    {
        s_output_reg_cache &= ~(1 << pin);
    }

    esp_err_t err = ESP_OK;
    if (s_output_reg_cache != prev_val)
    {
        err = s_pca9557_write_reg(REG_OUTPUT_PORT, s_output_reg_cache);
        if (err != ESP_OK)
        {
            s_output_reg_cache = prev_val; // 写入失败时还原影子寄存器
        }
    }

    xSemaphoreGive(s_pca9557_mutex);
    return err;
}

esp_err_t pca9557_get_output_level(uint8_t pin, uint8_t *out_level)
{
    if (pin > 7 || out_level == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_pca9557_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_pca9557_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    *out_level = (s_output_reg_cache >> pin) & 0x01;

    xSemaphoreGive(s_pca9557_mutex);
    return ESP_OK;
}

esp_err_t pca9557_get_input_level(uint8_t pin, uint8_t *out_level)
{
    if (pin > 7 || out_level == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_pca9557_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_pca9557_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t reg_val = 0;
    esp_err_t err = s_pca9557_read_reg(REG_INPUT_PORT, &reg_val);
    if (err == ESP_OK)
    {
        *out_level = (reg_val >> pin) & 0x01;
    }

    xSemaphoreGive(s_pca9557_mutex);
    return err;
}
