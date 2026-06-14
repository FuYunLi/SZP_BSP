#pragma once

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 启动 HTTP 流媒体服务器
 *
 * 启动HTTP服务器，提供 /stream 端点用于MJPEG视频流传输。
 * 需要先初始化WiFi并连接网络，摄像头会自动初始化。
 *
 * @return esp_err_t ESP_OK 表示成功启动，其他值表示失败
 */
esp_err_t http_stream_server_start(void);

/**
 * @brief 停止 HTTP 流媒体服务器
 *
 * 停止HTTP服务器，释放摄像头资源，停止捕获任务。
 *
 * @return esp_err_t ESP_OK 表示成功停止，其他值表示失败
 */
esp_err_t http_stream_server_stop(void);

/**
 * @brief 获取 HTTP 流媒体服务器运行状态
 *
 * @return bool true 表示服务器正在运行，false 表示已停止
 */
bool http_stream_server_is_running(void);

#ifdef __cplusplus
}
#endif