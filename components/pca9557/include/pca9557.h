#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

#define PCA9557_I2C_ADDR    (0x19)

/**
 * @brief 初始化 PCA9557 设备并加入 I2C 总线
 * 
 * @param bus_handle I2C 总线句柄
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t pca9557_init(i2c_master_bus_handle_t bus_handle);

/**
 * @brief 设置 PCA9557 引脚的输入/输出配置（Configuration Register）
 * 
 * @param pin 引脚号 (0-7)
 * @param is_input 是否为输入 (true: 输入, false: 输出)
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t pca9557_set_config(uint8_t pin, bool is_input);

/**
 * @brief 设置 PCA9557 引脚的输出电平 (Output Port Register)
 * 
 * @param pin 引脚号 (0-7)
 * @param level 输出电平 (0: 低电平, 1: 高电平)
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t pca9557_set_output_level(uint8_t pin, uint8_t level);

/**
 * @brief 获取 PCA9557 引脚的输出电平 (Output Port Register)
 * 
 * @param pin 引脚号 (0-7)
 * @param out_level 输出参数，存放读取到的输出电平值 (0/1)
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t pca9557_get_output_level(uint8_t pin, uint8_t *out_level);

/**
 * @brief 获取 PCA9557 引脚的输入电平 (Input Port Register)
 * 
 * @param pin 引脚号 (0-7)
 * @param out_level 输出参数，存放读取到的实际物理输入电平值 (0/1)
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t pca9557_get_input_level(uint8_t pin, uint8_t *out_level);
