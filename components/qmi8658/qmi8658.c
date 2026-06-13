#include "qmi8658.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "qmi8658";

static i2c_master_dev_handle_t s_imu_dev_handle = NULL;
static SemaphoreHandle_t s_imu_mutex = NULL;

/* QMI8658A 内部寄存器地址定义 */
#define QMI8658A_REG_WHO_AM_I   (0x00)
#define QMI8658A_REG_CTRL1      (0x02)
#define QMI8658A_REG_CTRL2      (0x03)
#define QMI8658A_REG_CTRL3      (0x04)
#define QMI8658A_REG_CTRL5      (0x06)
#define QMI8658A_REG_CTRL7      (0x08)
#define QMI8658A_REG_RESET      (0x60)

#define QMI8658A_REG_TEMP_L     (0x33)
#define QMI8658A_REG_AX_L       (0x35)

static esp_err_t s_imu_write_reg(uint8_t reg_addr, uint8_t value)
{
    if (s_imu_dev_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }
    uint8_t data[2] = {reg_addr, value};
    return i2c_master_transmit(s_imu_dev_handle, data, sizeof(data), 1000);
}

static esp_err_t s_imu_read_reg(uint8_t reg_addr, uint8_t *value)
{
    if (s_imu_dev_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_transmit_receive(s_imu_dev_handle, &reg_addr, 1, value, 1, 1000);
}

static esp_err_t s_imu_read_bytes(uint8_t start_addr, uint8_t *buffer, size_t length)
{
    if (s_imu_dev_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_transmit_receive(s_imu_dev_handle, &start_addr, 1, buffer, length, 1000);
}

esp_err_t qmi8658_init(i2c_master_bus_handle_t bus_handle)
{
    if (s_imu_dev_handle != NULL)
    {
        ESP_LOGW(TAG, "IMU already initialized");
        return ESP_OK;
    }

    if (bus_handle == NULL)
    {
        ESP_LOGE(TAG, "Invalid I2C bus handle passed to IMU init");
        return ESP_ERR_INVALID_ARG;
    }

    s_imu_mutex = xSemaphoreCreateMutex();
    if (s_imu_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_ERR_NO_MEM;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = QMI8658A_I2C_ADDR,
        .scl_speed_hz = 100000,
    };

    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_config, &s_imu_dev_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add IMU device to I2C bus: %s", esp_err_to_name(err));
        vSemaphoreDelete(s_imu_mutex);
        s_imu_mutex = NULL;
        return err;
    }

    // 软复位 QMI8658A 传感器以确保配置环境干净
    err = s_imu_write_reg(QMI8658A_REG_RESET, 0xB0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to send soft reset: %s", esp_err_to_name(err));
        return err;
    }
    vTaskDelay(pdMS_TO_TICKS(20)); // 复位后等待 20 毫秒

    // 读取 WHO_AM_I 验证设备连接
    uint8_t who_am_i = 0;
    err = s_imu_read_reg(QMI8658A_REG_WHO_AM_I, &who_am_i);
    if (err != ESP_OK || who_am_i != QMI8658A_WHO_AM_I_VAL)
    {
        ESP_LOGE(TAG, "Failed to verify WHO_AM_I: expected 0x%02X, got 0x%02X (err: %s)", 
                 QMI8658A_WHO_AM_I_VAL, who_am_i, esp_err_to_name(err));
        return (err == ESP_OK) ? ESP_ERR_INVALID_VERSION : err;
    }

    // 配置工作参数
    // CTRL1: 启用地址自动递增，小端格式
    err = s_imu_write_reg(QMI8658A_REG_CTRL1, QMI8658A_CTRL1_ADDR_AI);
    if (err != ESP_OK) return err;

    // CTRL2: 加速度计配置 (±8g 量程, ODR 250Hz)
    err = s_imu_write_reg(QMI8658A_REG_CTRL2, QMI8658A_ACC_FS_8G | QMI8658A_ACC_ODR_250HZ);
    if (err != ESP_OK) return err;

    // CTRL3: 陀螺仪配置 (±512 dps 量程, ODR 224.2Hz)
    err = s_imu_write_reg(QMI8658A_REG_CTRL3, QMI8658A_GYRO_FS_512DPS | QMI8658A_GYRO_ODR_224HZ);
    if (err != ESP_OK) return err;

    // CTRL5: 使能低通滤波器 (加速度计与陀螺仪均使能，截止频率 2.66% ODR)
    err = s_imu_write_reg(QMI8658A_REG_CTRL5, QMI8658A_CTRL5_ACC_LPF_EN | QMI8658A_CTRL5_GYRO_LPF_EN);
    if (err != ESP_OK) return err;

    // CTRL7: 同时使能加速度计 (aEN) 和陀螺仪 (gEN)
    err = s_imu_write_reg(QMI8658A_REG_CTRL7, QMI8658A_CTRL7_ACC_EN | QMI8658A_CTRL7_GYRO_EN);
    if (err != ESP_OK) return err;

    ESP_LOGI(TAG, "QMI8658A IMU initialized successfully (WHO_AM_I: 0x%02X)", who_am_i);
    return ESP_OK;
}

esp_err_t qmi8658_read_raw(int16_t acc_raw[3], int16_t gyro_raw[3])
{
    if (s_imu_dev_handle == NULL || s_imu_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_imu_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t buffer[12];
    esp_err_t err = s_imu_read_bytes(QMI8658A_REG_AX_L, buffer, 12);
    if (err == ESP_OK)
    {
        // 高位和低位根据 Little-Endian 端序拼接
        acc_raw[0] = (int16_t)(buffer[1] << 8 | buffer[0]);
        acc_raw[1] = (int16_t)(buffer[3] << 8 | buffer[2]);
        acc_raw[2] = (int16_t)(buffer[5] << 8 | buffer[4]);
        gyro_raw[0] = (int16_t)(buffer[7] << 8 | buffer[6]);
        gyro_raw[1] = (int16_t)(buffer[9] << 8 | buffer[8]);
        gyro_raw[2] = (int16_t)(buffer[11] << 8 | buffer[10]);
    }

    xSemaphoreGive(s_imu_mutex);
    return err;
}

esp_err_t qmi8658_read_parsed(float acc_g[3], float gyro_dps[3])
{
    int16_t acc_raw[3];
    int16_t gyro_raw[3];

    esp_err_t err = qmi8658_read_raw(acc_raw, gyro_raw);
    if (err != ESP_OK)
    {
        return err;
    }

    // 加速度计配置为 ±8g 量程，对应的灵敏度为 4096 LSB/g
    acc_g[0] = (float)acc_raw[0] / 4096.0f;
    acc_g[1] = (float)acc_raw[1] / 4096.0f;
    acc_g[2] = (float)acc_raw[2] / 4096.0f;

    // 陀螺仪配置为 ±512 dps 量程，对应的灵敏度为 64 LSB/dps
    gyro_dps[0] = (float)gyro_raw[0] / 64.0f;
    gyro_dps[1] = (float)gyro_raw[1] / 64.0f;
    gyro_dps[2] = (float)gyro_raw[2] / 64.0f;

    return ESP_OK;
}

esp_err_t qmi8658_read_temp(float *temp_c)
{
    if (temp_c == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_imu_dev_handle == NULL || s_imu_mutex == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(s_imu_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t buffer[2];
    esp_err_t err = s_imu_read_bytes(QMI8658A_REG_TEMP_L, buffer, 2);
    if (err == ESP_OK)
    {
        int16_t temp_raw = (int16_t)(buffer[1] << 8 | buffer[0]);
        // 芯片温度分辨率为 256 LSB/℃
        *temp_c = (float)temp_raw / 256.0f;
    }

    xSemaphoreGive(s_imu_mutex);
    return err;
}
