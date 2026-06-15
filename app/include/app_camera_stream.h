/**
 * @file app_camera_stream.h
 * @brief 摄像头流媒体应用接口声明
 *
 * 实现摄像头捕获、JPEG编码和双缓冲机制，
 * 通过调用http_stream_service提供HTTP流媒体传输功能。
 */

#pragma once

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 启动摄像头流媒体应用
 *
 * 初始化摄像头，启动捕获任务，并启动HTTP流媒体服务。
 *
 * @return esp_err_t ESP_OK表示成功启动，其他值表示失败
 */
esp_err_t app_camera_stream_start(void);

/**
 * @brief 停止摄像头流媒体应用
 *
 * 停止HTTP流媒体服务，停止捕获任务，释放摄像头资源。
 *
 * @return esp_err_t ESP_OK表示成功停止，其他值表示失败
 */
esp_err_t app_camera_stream_stop(void);

/**
 * @brief 获取摄像头流媒体应用运行状态
 *
 * @return bool true表示应用正在运行，false表示已停止
 */
bool app_camera_stream_is_running(void);

#ifdef __cplusplus
}
#endif