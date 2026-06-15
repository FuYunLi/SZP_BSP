/**
 * @file bsp_audio.h
 * @brief 板载 I2S 音频接口与功放控制板级支持包头文件
 */

#pragma once

#include "esp_err.h"
#include "driver/i2s_std.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板级音频 I2S0 标准通道及配置功放引脚
 * 
 * @note 此函数会初始化 I2S0 控制器为 Master 模式，映射 IO 引脚，
 *       并通过 PCA9557 将功放使能脚 PA_EN 置为低电平（等待起播防杂音）。
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_audio_i2s_init(void);

/**
 * @brief 板级音频功放使能控制
 * 
 * @param[in] enable true 表示开启功放使能，false 表示关闭功放以降低待机功耗并防范杂音
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_audio_pa_enable(bool enable);

/**
 * @brief 获取已注册的 I2S TX 通道句柄
 * 
 * @return i2s_chan_handle_t 返回 I2S TX 通道句柄，如果未初始化则返回 NULL
 */
i2s_chan_handle_t bsp_audio_get_tx_handle(void);

/**
 * @brief 获取已注册的 I2S RX 通道句柄
 * 
 * @return i2s_chan_handle_t 返回 I2S RX 通道句柄，如果未初始化则返回 NULL
 */
i2s_chan_handle_t bsp_audio_get_rx_handle(void);
 
/**
 * @brief 设置板载音频 Codec 输出音量
 * 
 * @param[in] volume 音量大小 (0 ~ 100)
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_audio_volume_set(uint8_t volume);

/**
 * @brief 从板载麦克风读取原始音频数据
 * 
 * @param[out] dest 接收缓冲区指针
 * @param[in] size 期望读取的字节数
 * @param[out] bytes_read 实际读取到的字节数
 * @param[in] timeout_ms 读取超时时间 (毫秒)
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_audio_record_read(void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms);

/**
 * @brief 反初始化板级音频 I2S 通道及释放所有资源
 * 
 * @note 此函数会关闭功放、删除 I2S 通道、删除 Codec 和 ADC 句柄
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_audio_i2s_deinit(void);

#ifdef __cplusplus
}
#endif
