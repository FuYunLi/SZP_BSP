/**
 * @file http_stream_service.c
 * @brief HTTP流媒体服务实现
 *
 * 提供HTTP流媒体服务器功能，支持MJPEG视频流传输。
 * 该服务层模块负责HTTP服务器管理，应用层通过回调接口注入业务逻辑。
 */

#include "http_stream_service.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "http_stream_service";

/* ================================================================
 * 私有变量
 * ================================================================ */

// HTTP服务器句柄
static httpd_handle_t s_server = NULL;

// 运行标志
static bool s_running = false;

// 帧数据提供回调函数（由应用层注入）
static http_stream_frame_provider_t s_frame_provider = NULL;

// 客户端计数器
static int s_client_count = 0;

/* ================================================================
 * HTTP端点处理函数
 * ================================================================ */

/**
 * @brief HTTP /stream 端点处理函数
 *
 * 处理客户端的/stream请求，循环发送JPEG帧数据。
 */
static esp_err_t stream_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "New client connected to /stream");

    // 设置响应头（MJPEG流）
    httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    httpd_resp_set_hdr(req, "Pragma", "no-cache");

    // 增加客户端计数
    s_client_count++;

    // 循环发送JPEG帧
    while (s_running && s_client_count > 0)
    {
        // 检查帧数据提供回调函数是否有效
        if (s_frame_provider == NULL)
        {
            ESP_LOGW(TAG, "Frame provider callback is NULL");
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // 调用应用层提供的帧数据回调函数
        uint8_t *jpeg_buf = NULL;
        size_t jpeg_size = 0;
        esp_err_t ret = s_frame_provider(&jpeg_buf, &jpeg_size);

        if (ret != ESP_OK || jpeg_size == 0 || jpeg_buf == NULL)
        {
            ESP_LOGW(TAG, "Failed to get frame data from provider");
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // 发送boundary分隔符
        char boundary_header[128];
        snprintf(boundary_header, sizeof(boundary_header),
                 "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %zu\r\n\r\n",
                 jpeg_size);

        ret = httpd_resp_send_chunk(req, boundary_header, strlen(boundary_header));
        if (ret != ESP_OK)
        {
            ESP_LOGW(TAG, "Failed to send boundary header: %s", esp_err_to_name(ret));
            break;
        }

        // 发送JPEG帧数据
        ret = httpd_resp_send_chunk(req, (char *)jpeg_buf, jpeg_size);
        if (ret != ESP_OK)
        {
            ESP_LOGW(TAG, "Failed to send JPEG frame: %s", esp_err_to_name(ret));
            break;
        }

        // 发送结束标记
        ret = httpd_resp_send_chunk(req, "\r\n", 2);
        if (ret != ESP_OK)
        {
            ESP_LOGW(TAG, "Failed to send end marker: %s", esp_err_to_name(ret));
            break;
        }

        // 控制帧率（10 FPS）
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // 减少客户端计数
    s_client_count--;
    ESP_LOGI(TAG, "Client disconnected from /stream");

    return ESP_OK;
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

esp_err_t http_stream_service_start(http_stream_frame_provider_t provider)
{
    // 检查是否已运行
    if (s_running || s_server != NULL)
    {
        ESP_LOGW(TAG, "HTTP stream service already running");
        return ESP_OK;
    }

    // 检查帧数据提供回调函数是否有效
    if (provider == NULL)
    {
        ESP_LOGE(TAG, "Frame provider callback is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    // 保存帧数据提供回调函数
    s_frame_provider = provider;

    // 配置HTTP服务器（使用8080端口避免冲突）
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 8080;  // 使用8080端口，避免与系统服务冲突
    config.ctrl_port = 8081;
    config.max_open_sockets = 4;
    config.lru_purge_enable = true;
    config.stack_size = 8192;

    // 启动HTTP服务器
    esp_err_t ret = httpd_start(&s_server, &config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTP server: %s", esp_err_to_name(ret));
        s_server = NULL;
        s_frame_provider = NULL;
        return ret;
    }

    // 注册 /stream URI处理函数
    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL};

    ret = httpd_register_uri_handler(s_server, &stream_uri);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register /stream handler: %s", esp_err_to_name(ret));
        httpd_stop(s_server);
        s_server = NULL;
        s_frame_provider = NULL;
        return ret;
    }

    // 设置运行标志
    s_running = true;

    ESP_LOGI(TAG, "HTTP stream service started successfully");
    ESP_LOGI(TAG, "Access video stream at: http://<device_ip>:8080/stream");

    return ESP_OK;
}

esp_err_t http_stream_service_stop(void)
{
    // 检查是否已停止
    if (!s_running && s_server == NULL)
    {
        ESP_LOGW(TAG, "HTTP stream service already stopped");
        return ESP_OK;
    }

    // 停止运行标志
    s_running = false;

    // 停止HTTP服务器
    if (s_server != NULL)
    {
        httpd_stop(s_server);
        s_server = NULL;
    }

    // 清除帧数据提供回调函数
    s_frame_provider = NULL;

    // 重置客户端计数
    s_client_count = 0;

    ESP_LOGI(TAG, "HTTP stream service stopped successfully");

    return ESP_OK;
}

bool http_stream_service_is_running(void)
{
    return s_running;
}