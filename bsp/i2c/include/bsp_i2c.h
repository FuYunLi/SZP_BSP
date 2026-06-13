#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

#define BSP_I2C_SDA_PIN   (1)
#define BSP_I2C_SCL_PIN   (2)
#define BSP_I2C_FREQ_HZ   (100000)

/**
 * @brief 初始化板载 I2C1 主总线
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t bsp_i2c_init(void);

/**
 * @brief 获取板载 I2C1 主总线的句柄
 * 
 * @return i2c_master_bus_handle_t I2C 总线句柄，若未初始化则返回 NULL
 */
i2c_master_bus_handle_t bsp_i2c_get_bus_handle(void);

/**
 * @brief 反初始化板载 I2C1 主总线
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t bsp_i2c_deinit(void);
