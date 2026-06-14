/**
 * @file es7210.h
 * @brief ES7210 音频 ADC 极简本地驱动头文件 (基于 driver_ng I2C 总线接口)
 */

#ifndef ES7210_H
#define ES7210_H

#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* es7210_handle_t;

/* ES7210 7位默认 I2C 设备地址 */
#define ES7210_AD0_AD1_00_ADDR    0x40 // AD0=0, AD1=0
#define ES7210_AD0_AD1_01_ADDR    0x41 // AD0=1, AD1=0 (立创实战派 S3 所用地址)
#define ES7210_AD0_AD1_10_ADDR    0x42 // AD0=0, AD1=1
#define ES7210_AD0_AD1_11_ADDR    0x43 // AD0=1, AD1=1

/* ES7210 模拟输入麦克风通道选择掩码 */
typedef enum {
    ES7210_INPUT_MIC1 = 0x01,
    ES7210_INPUT_MIC2 = 0x02,
    ES7210_INPUT_MIC3 = 0x04,
    ES7210_INPUT_MIC4 = 0x08,
} es7210_mic_select_t;

/* 芯片初始化配置参数 */
typedef struct {
    uint8_t mic_selected; // 启用的麦克风引脚掩码，例：ES7210_INPUT_MIC1 | ES7210_INPUT_MIC2
    float gain_db;        // 通道模拟输入增益 (0dB ~ 37.5dB)
    bool master_mode;     // 是否配置为主模式 (在立创开发板中作为 I2S 从机，故此处设为 false)
} es7210_config_t;

/**
 * @brief 创建并注册一个 ES7210 芯片设备句柄
 * 
 * @param bus_handle 已初始化的 I2C master 总线句柄
 * @param dev_addr 7位 I2C 外设地址 (推荐使用 ES7210_AD0_AD1_01_ADDR 即 0x41)
 * @return es7210_handle_t 返回设备驱动句柄，失败返回 NULL
 */
es7210_handle_t es7210_create(i2c_master_bus_handle_t bus_handle, uint16_t dev_addr);

/**
 * @brief 初始化 ES7210 芯片寄存器 (进入工作状态)
 * 
 * @param dev 设备句柄
 * @param cfg 初始化配置项
 * @return esp_err_t ESP_OK 表示成功，其它为失败
 */
esp_err_t es7210_init(es7210_handle_t dev, const es7210_config_t *cfg);

/**
 * @brief 动态切换/选择启用的麦克风输入通道
 * 
 * @param dev 设备句柄
 * @param mic_mask 麦克风选择掩码
 * @return esp_err_t 
 */
esp_err_t es7210_mic_select(es7210_handle_t dev, uint8_t mic_mask);

/**
 * @brief 设置麦克风模拟输入增益
 * 
 * @param dev 设备句柄
 * @param gain_db 模拟增益大小 (0dB ~ 37.5dB)
 * @return esp_err_t 
 */
esp_err_t es7210_set_gain(es7210_handle_t dev, float gain_db);

/**
 * @brief 设置或解除麦克风数字输入静音
 * 
 * @param dev 设备句柄
 * @param mute true 表示静音，false 表示取消静音
 * @return esp_err_t 
 */
esp_err_t es7210_set_mute(es7210_handle_t dev, bool mute);

/**
 * @brief 注销并注销 ES7210 芯片设备
 * 
 * @param dev 设备句柄
 * @return esp_err_t 
 */
esp_err_t es7210_delete(es7210_handle_t dev);

#ifdef __cplusplus
}
#endif

#endif // ES7210_H
