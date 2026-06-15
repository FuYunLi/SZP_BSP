/**
 * @file http_stream_service.h
 * @brief HTTP流媒体服务接口声明
 *
 * 提供HTTP流媒体服务器功能，支持MJPEG视频流传输。
 * 该服务层模块负责HTTP服务器管理，应用层通过回调接口注入业务逻辑。
 */

#pragma once

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 帧数据提供回调函数类型
 *
 * 应用层通过此回调函数向服务层提供JPEG帧数据。
 *
 * @param[out] jpeg_buf JPEG帧数据缓冲区指针
 * @param[out] jpeg_size JPEG帧数据大小
 * @return esp_err_t ESP_OK表示成功，其他值表示失败
 */
typedef esp_err_t (*http_stream_frame_provider_t)(uint8_t **jpeg_buf, size_t *jpeg_size);

/**
 * @brief 启动HTTP流媒体服务
 *
 * 启动HTTP服务器，提供 /stream 端点用于MJPEG视频流传输。
 * 需要先初始化WiFi并连接网络。
 *
 * @param[in] provider 帧数据提供回调函数（由应用层注入）
 * @return esp_err_t ESP_OK表示成功启动，其他值表示失败
 */
esp_err_t http_stream_service_start(http_stream_frame_provider_t provider);

/**
 * @brief 停止HTTP流媒体服务
 *
 * 停止HTTP服务器，释放资源。
 *
 * @return esp_err_t ESP_OK表示成功停止，其他值表示失败
 */
esp_err_t http_stream_service_stop(void);

/**
 * @brief 获取HTTP流媒体服务运行状态
 *
 * @return bool true表示服务正在运行，false表示已停止
 */
bool http_stream_service_is_running(void);

#ifdef __cplusplus
}
#endif