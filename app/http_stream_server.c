#include "http_stream_server.h"
#include "bsp_camera.h"
#include "esp_http_server.h"
#include "esp_jpeg_enc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "wifi_service.h"
#include <string.h>

static const char *TAG = "http_stream";

// HTTP服务器句柄
static httpd_handle_t s_server = NULL;

// 摄像头捕获任务句柄
static TaskHandle_t s_capture_task = NULL;

// JPEG编码缓冲区（双缓冲机制）
static uint8_t *s_jpeg_buf[2] = {NULL, NULL};
static size_t s_jpeg_size[2] = {0, 0};
static int s_current_buf_index = 0;

// 互斥锁（保护JPEG缓冲区）
static SemaphoreHandle_t s_buf_mutex = NULL;

// 运行标志
static bool s_running = false;

// 客户端计数器
static int s_client_count = 0;

// JPEG编码配置
#define JPEG_BUFFER_SIZE 65536 // JPEG缓冲区大小（64KB）

// 摄像头捕获任务
static void camera_capture_task(void *arg) {
  ESP_LOGI(TAG, "Camera capture task started");

  // JPEG编码器配置
  jpeg_enc_config_t jpeg_cfg = {
      .width = 320,
      .height = 240,
      .src_type = JPEG_PIXEL_FORMAT_RGB565_LE, // RGB565小端格式
      .subsampling = JPEG_SUBSAMPLE_420,       // YUV420子采样
      .quality = 60,                           // JPEG质量（1-100）
      .rotate = JPEG_ROTATE_0D,                // 不旋转
      .task_enable = false,                    // 单任务模式
  };

  // 打开JPEG编码器
  jpeg_enc_handle_t jpeg_enc = NULL;
  jpeg_error_t ret = jpeg_enc_open(&jpeg_cfg, &jpeg_enc);
  if (ret != JPEG_ERR_OK) {
    ESP_LOGE(TAG, "Failed to open JPEG encoder: %d", ret);
    s_running = false;
    vTaskDelete(NULL);
    return;
  }

  while (s_running) {
    // 捕获摄像头帧
    camera_fb_t *fb = bsp_camera_fb_get();
    if (fb == NULL) {
      ESP_LOGW(TAG, "Camera frame capture failed");
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // JPEG编码（RGB565转JPEG）
    int next_buf_index = (s_current_buf_index + 1) % 2;
    int out_size = 0;

    // 执行JPEG编码
    ret =
        jpeg_enc_process(jpeg_enc, fb->buf, fb->len, s_jpeg_buf[next_buf_index],
                         JPEG_BUFFER_SIZE, &out_size);

    if (ret != JPEG_ERR_OK) {
      ESP_LOGW(TAG, "JPEG encoding failed: %d", ret);
      bsp_camera_fb_return(fb);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // 更新JPEG帧大小
    s_jpeg_size[next_buf_index] = out_size;

    // 获取互斥锁，更新缓冲区索引
    if (xSemaphoreTake(s_buf_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      s_current_buf_index = next_buf_index;
      xSemaphoreGive(s_buf_mutex);
    }

    // 释放摄像头帧
    bsp_camera_fb_return(fb);

    // 控制帧率（10 FPS）
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  // 关闭JPEG编码器
  jpeg_enc_close(jpeg_enc);

  ESP_LOGI(TAG, "Camera capture task stopped");
  vTaskDelete(NULL);
}

// HTTP /stream 端点处理函数
static esp_err_t stream_handler(httpd_req_t *req) {
  ESP_LOGI(TAG, "New client connected to /stream");

  // 设置响应头（MJPEG流）
  httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
  httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
  httpd_resp_set_hdr(req, "Pragma", "no-cache");

  // 增加客户端计数
  s_client_count++;

  // 循环发送JPEG帧
  while (s_running && s_client_count > 0) {
    // 获取互斥锁，读取最新JPEG帧
    if (xSemaphoreTake(s_buf_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }

    int buf_index = s_current_buf_index;
    size_t jpeg_size = s_jpeg_size[buf_index];
    uint8_t *jpeg_buf = s_jpeg_buf[buf_index];

    xSemaphoreGive(s_buf_mutex);

    // 检查JPEG帧是否有效
    if (jpeg_size == 0 || jpeg_buf == NULL) {
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // 发送boundary分隔符
    char boundary_header[128];
    snprintf(
        boundary_header, sizeof(boundary_header),
        "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %zu\r\n\r\n",
        jpeg_size);

    esp_err_t ret =
        httpd_resp_send_chunk(req, boundary_header, strlen(boundary_header));
    if (ret != ESP_OK) {
      ESP_LOGW(TAG, "Failed to send boundary header: %s", esp_err_to_name(ret));
      break;
    }

    // 发送JPEG帧数据
    ret = httpd_resp_send_chunk(req, (char *)jpeg_buf, jpeg_size);
    if (ret != ESP_OK) {
      ESP_LOGW(TAG, "Failed to send JPEG frame: %s", esp_err_to_name(ret));
      break;
    }

    // 发送结束标记
    ret = httpd_resp_send_chunk(req, "\r\n", 2);
    if (ret != ESP_OK) {
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

// 启动HTTP服务器
esp_err_t http_stream_server_start(void) {
  // 检查是否已运行
  if (s_running) {
    ESP_LOGW(TAG, "HTTP stream server already running");
    return ESP_OK;
  }

  // 检查WiFi连接状态
  wifi_service_state_t wifi_state = wifi_service_get_state();
  if (wifi_state != WIFI_SERVICE_STATE_GOT_IP) {
    ESP_LOGE(TAG, "WiFi not connected, cannot start HTTP server");
    return ESP_ERR_INVALID_STATE;
  }

  // 初始化摄像头（如果未初始化）
  esp_err_t ret = bsp_camera_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Camera initialization failed: %s", esp_err_to_name(ret));
    return ret;
  }

  // 分配JPEG编码缓冲区（双缓冲）
  s_jpeg_buf[0] = malloc(JPEG_BUFFER_SIZE);
  s_jpeg_buf[1] = malloc(JPEG_BUFFER_SIZE);
  if (s_jpeg_buf[0] == NULL || s_jpeg_buf[1] == NULL) {
    ESP_LOGE(TAG, "Failed to allocate JPEG buffers");
    free(s_jpeg_buf[0]);
    free(s_jpeg_buf[1]);
    return ESP_ERR_NO_MEM;
  }

  // 创建互斥锁
  s_buf_mutex = xSemaphoreCreateMutex();
  if (s_buf_mutex == NULL) {
    ESP_LOGE(TAG, "Failed to create mutex");
    free(s_jpeg_buf[0]);
    free(s_jpeg_buf[1]);
    return ESP_ERR_NO_MEM;
  }

  // 启动摄像头捕获任务
  s_running = true;
  BaseType_t task_ret = xTaskCreatePinnedToCore(
      camera_capture_task, "camera_capture", 8192, NULL, 5, &s_capture_task, 1);

  if (task_ret != pdPASS) {
    ESP_LOGE(TAG, "Failed to create camera capture task");
    s_running = false;
    vSemaphoreDelete(s_buf_mutex);
    free(s_jpeg_buf[0]);
    free(s_jpeg_buf[1]);
    return ESP_ERR_NO_MEM;
  }

  // 配置HTTP服务器
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  config.ctrl_port = 8080;
  config.max_open_sockets = 4;
  config.lru_purge_enable = true;
  config.stack_size = 8192;

  // 启动HTTP服务器
  ret = httpd_start(&s_server, &config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start HTTP server: %s", esp_err_to_name(ret));
    s_running = false;
    vTaskDelete(s_capture_task);
    vSemaphoreDelete(s_buf_mutex);
    free(s_jpeg_buf[0]);
    free(s_jpeg_buf[1]);
    return ret;
  }

  // 注册 /stream URI处理函数
  httpd_uri_t stream_uri = {.uri = "/stream",
                            .method = HTTP_GET,
                            .handler = stream_handler,
                            .user_ctx = NULL};

  ret = httpd_register_uri_handler(s_server, &stream_uri);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to register /stream handler: %s",
             esp_err_to_name(ret));
    httpd_stop(s_server);
    s_server = NULL;
    s_running = false;
    vTaskDelete(s_capture_task);
    vSemaphoreDelete(s_buf_mutex);
    free(s_jpeg_buf[0]);
    free(s_jpeg_buf[1]);
    return ret;
  }

  ESP_LOGI(TAG, "HTTP stream server started successfully");
  ESP_LOGI(TAG, "Access video stream at: http://<device_ip>/stream");

  return ESP_OK;
}

// 停止HTTP服务器
esp_err_t http_stream_server_stop(void) {
  // 检查是否已停止
  if (!s_running) {
    ESP_LOGW(TAG, "HTTP stream server already stopped");
    return ESP_OK;
  }

  // 停止运行标志
  s_running = false;

  // 等待摄像头捕获任务结束
  if (s_capture_task != NULL) {
    vTaskDelay(pdMS_TO_TICKS(200));
    s_capture_task = NULL;
  }

  // 停止HTTP服务器
  if (s_server != NULL) {
    httpd_stop(s_server);
    s_server = NULL;
  }

  // 释放JPEG缓冲区
  free(s_jpeg_buf[0]);
  free(s_jpeg_buf[1]);
  s_jpeg_buf[0] = NULL;
  s_jpeg_buf[1] = NULL;
  s_jpeg_size[0] = 0;
  s_jpeg_size[1] = 0;

  // 删除互斥锁
  if (s_buf_mutex != NULL) {
    vSemaphoreDelete(s_buf_mutex);
    s_buf_mutex = NULL;
  }

  // 重置客户端计数
  s_client_count = 0;

  ESP_LOGI(TAG, "HTTP stream server stopped successfully");

  return ESP_OK;
}

// 获取服务器运行状态
bool http_stream_server_is_running(void) { return s_running; }