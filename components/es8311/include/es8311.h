#pragma once

#include "esp_types.h"
#include "esp_err.h"
#include "driver/i2c_master.h"

#define ES8311_ADDRESS_0 0x18u
#define ES8311_ADDRESS_1 0x19u

#ifdef __cplusplus
extern "C" {
#endif

typedef void *es8311_handle_t;

typedef enum es8311_resolution_t {
    ES8311_RESOLUTION_16 = 16,
    ES8311_RESOLUTION_18 = 18,
    ES8311_RESOLUTION_20 = 20,
    ES8311_RESOLUTION_24 = 24,
    ES8311_RESOLUTION_32 = 32
} es8311_resolution_t;

typedef struct es8311_clock_config_t {
    bool mclk_inverted;
    bool sclk_inverted;
    bool mclk_from_mclk_pin; // true: from MCLK pin, false: from SCLK pin
    int  mclk_frequency;     // MCLK 频率
    int  sample_frequency;   // 采样率 (Hz)
} es8311_clock_config_t;

/**
 * @brief 创建 ES8311 设备实例
 * 
 * @param bus_handle 新一代 I2C 总线句柄
 * @param dev_addr I2C 设备地址 (通常为 ES8311_ADDRESS_0)
 * @return es8311_handle_t 返回设备句柄，失败返回 NULL
 */
es8311_handle_t es8311_create(i2c_master_bus_handle_t bus_handle, const uint16_t dev_addr);

/**
 * @brief 销毁 ES8311 设备实例
 */
void es8311_delete(es8311_handle_t dev);

/**
 * @brief 初始化 ES8311 芯片寄存器
 */
esp_err_t es8311_init(es8311_handle_t dev, const es8311_clock_config_t *const clk_cfg, 
                      const es8311_resolution_t res_in, const es8311_resolution_t res_out);

/**
 * @brief 设置输出音量 (0 ~ 100)
 */
esp_err_t es8311_voice_volume_set(es8311_handle_t dev, int volume, int *volume_set);

/**
 * @brief 获取输出音量 (0 ~ 100)
 */
esp_err_t es8311_voice_volume_get(es8311_handle_t dev, int *volume);

/**
 * @brief 控制静音
 */
esp_err_t es8311_voice_mute(es8311_handle_t dev, bool mute);

#ifdef __cplusplus
}
#endif
