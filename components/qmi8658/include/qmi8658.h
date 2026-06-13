#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdint.h>

/* QMI8658A I2C 地址 (SA0 接地时为 0x6A) */
#define QMI8658A_I2C_ADDR       (0x6A)

/* QMI8658A 芯片 ID */
#define QMI8658A_WHO_AM_I_VAL   (0x05)

/* CTRL1 寄存器位定义 */
#define QMI8658A_CTRL1_ADDR_AI  (1 << 6) // I2C/SPI 地址自动递增
#define QMI8658A_CTRL1_BE       (1 << 5) // 大端格式 (0为小端，1为大端)

/* CTRL2 加速度计配置 (±8g 量程, 250Hz ODR) */
#define QMI8658A_ACC_FS_8G      (2 << 4) // ±8g (0:2g, 1:4g, 2:8g, 3:16g)
#define QMI8658A_ACC_ODR_250HZ  (5 << 0) // 250Hz

/* CTRL3 陀螺仪配置 (±512dps 量程, 224.2Hz ODR) */
#define QMI8658A_GYRO_FS_512DPS  (5 << 4) // ±512 dps (0:16dps, 1:32dps, 2:64dps, 3:128dps, 4:256dps, 5:512dps)
#define QMI8658A_GYRO_ODR_224HZ  (5 << 0) // 224.2Hz

/* CTRL5 低通滤波器配置 (启用且截止频率设为 2.66% ODR) */
#define QMI8658A_CTRL5_ACC_LPF_EN  (1 << 0)
#define QMI8658A_CTRL5_GYRO_LPF_EN (1 << 4)

/* CTRL7 使能配置 */
#define QMI8658A_CTRL7_ACC_EN   (1 << 0) // 使能加速度计
#define QMI8658A_CTRL7_GYRO_EN  (1 << 1) // 使能陀螺仪

/**
 * @brief 初始化板载 QMI8658A 传感器并加入指定 I2C 总线
 * 
 * @param bus_handle I2C 总线句柄
 * @return esp_err_t ESP_OK 表示成功，其它值表示错误
 */
esp_err_t qmi8658_init(i2c_master_bus_handle_t bus_handle);

/**
 * @brief 读取 IMU 加速度计与陀螺仪的原始 16 位 2 补码数据
 * 
 * @param acc_raw  长度为 3 的数组，分别存放 X, Y, Z 轴的原始数据
 * @param gyro_raw 长度为 3 的数组，分别存放 X, Y, Z 轴的原始数据
 * @return esp_err_t 
 */
esp_err_t qmi8658_read_raw(int16_t acc_raw[3], int16_t gyro_raw[3]);

/**
 * @brief 读取并转换为物理单位的加速度 (g) 与角速度 (dps)
 * 
 * @param acc_g    输出参数，X, Y, Z 轴加速度 (单位: g)
 * @param gyro_dps 输出参数，X, Y, Z 轴角速度 (单位: 度/秒)
 * @return esp_err_t 
 */
esp_err_t qmi8658_read_parsed(float acc_g[3], float gyro_dps[3]);

/**
 * @brief 读取传感器内部温度
 * 
 * @param temp_c 温度值 (℃)
 * @return esp_err_t 
 */
esp_err_t qmi8658_read_temp(float *temp_c);
