/**
 * @file app_camera_stream.c
 * @brief 摄像头流媒体应用实现
 *
 * 实现摄像头捕获、JPEG编码和双缓冲机制，
 * 通过调用http_stream_service提供HTTP流媒体传输功能。
 */

#include "app_camera_stream.h"
#include "bsp_camera.h"
#include "esp_camera.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "http_stream_service.h"
#include "img_converters.h"
#include <string.h>

static const char *TAG = "app_camera_stream";

/* ================================================================
 * 私有变量
 * ================================================================ */

// 运行标志
static bool s_running = false;

// 摄像头捕获任务句柄
static TaskHandle_t s_capture_task_handle = NULL;

// 双缓冲机制：两个JPEG缓冲区（动态分配在PSRAM中）
#define JPEG_BUF_SIZE (50 * 1024) // 50KB JPEG缓冲区
static uint8_t *s_jpeg_buf0 = NULL;
static uint8_t *s_jpeg_buf1 = NULL;
static size_t s_jpeg_size0 = 0;
static size_t s_jpeg_size1 = 0;

// 当前缓冲区索引（0或1）
static int s_current_buf_index = 0;

// 缓冲区互斥锁
static SemaphoreHandle_t s_buf_mutex = NULL;

/* ================================================================
 * 私有函数：JPEG编码
 * ================================================================ */

/**
 * @brief 将RGB565帧编码为JPEG格式
 *
 * 使用esp32-camera组件的fmt2jpg函数进行JPEG编码。
 *
 * @param[in] rgb_buf RGB565帧数据缓冲区
 * @param[in] width 图像宽度
 * @param[in] height 图像高度
 * @param[out] jpeg_buf JPEG帧数据缓冲区（预分配）
 * @param[in] jpeg_buf_size JPEG缓冲区大小
 * @param[out] jpeg_size JPEG帧数据大小
 * @return esp_err_t ESP_OK表示成功，其他值表示失败
 */
static esp_err_t s_encode_rgb565_to_jpeg(uint8_t *rgb_buf, uint16_t width,
                                         uint16_t height, uint8_t *jpeg_buf,
                                         size_t jpeg_buf_size,
                                         size_t *jpeg_size) {
  // 使用fmt2jpg进行JPEG编码
  // 注意：fmt2jpg会动态分配内存，我们需要复制到静态缓冲区
  uint8_t *jpeg_out = NULL;
  size_t jpeg_out_len = 0;

  // 设置RGB565为大端格式（修正颜色异常）
  jpgSetRgb565BE(true);

  // 执行JPEG编码，质量为12（1-31，越小质量越高）
  bool success = fmt2jpg(rgb_buf, width * height * 2, width, height,
                         PIXFORMAT_RGB565, 12, &jpeg_out, &jpeg_out_len);

  if (!success || jpeg_out == NULL || jpeg_out_len == 0) {
    ESP_LOGE(TAG, "JPEG encoding failed");
    return ESP_FAIL;
  }

  // 检查输出缓冲区是否足够大
  if (jpeg_out_len > jpeg_buf_size) {
    ESP_LOGE(TAG, "JPEG output too large: %zu > %zu", jpeg_out_len,
             jpeg_buf_size);
    free(jpeg_out);
    return ESP_ERR_NO_MEM;
  }

  // 复制JPEG数据到静态缓冲区
  memcpy(jpeg_buf, jpeg_out, jpeg_out_len);
  *jpeg_size = jpeg_out_len;

  // 释放动态分配的内存
  free(jpeg_out);

  ESP_LOGD(TAG, "JPEG encoded: %zu bytes", *jpeg_size);
  return ESP_OK;
}

/* ================================================================
 * 私有函数：摄像头捕获任务
 * ================================================================ */

/**
 * @brief 摄像头捕获任务
 *
 * 持续捕获摄像头帧，编码为JPEG格式，更新双缓冲区。
 */
static void s_camera_capture_task(void *arg) {
  ESP_LOGI(TAG, "Camera capture task started");

  while (s_running) {
    // 捕获摄像头帧
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb == NULL) {
      ESP_LOGW(TAG, "Camera frame capture failed");
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    ESP_LOGD(TAG, "Captured frame: %zu bytes, format: %d", fb->len, fb->format);

    // 检查帧格式（应该是RGB565）
    if (fb->format != PIXFORMAT_RGB565) {
      ESP_LOGW(TAG, "Unexpected frame format: %d, expected RGB565", fb->format);
      esp_camera_fb_return(fb);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // 选择下一个缓冲区（双缓冲）- 先确定目标缓冲区
    int next_buf_index = (s_current_buf_index == 0) ? 1 : 0;
    uint8_t *jpeg_buf = (next_buf_index == 0) ? s_jpeg_buf0 : s_jpeg_buf1;
    size_t jpeg_size = 0;

    // JPEG编码（不持有互斥锁，避免阻塞帧提供回调）
    esp_err_t ret = s_encode_rgb565_to_jpeg(
        fb->buf, fb->width, fb->height, jpeg_buf, JPEG_BUF_SIZE, &jpeg_size);
    if (ret != ESP_OK) {
      ESP_LOGW(TAG, "JPEG encoding failed: %s", esp_err_to_name(ret));
      esp_camera_fb_return(fb);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // 获取互斥锁（仅在更新缓冲区索引时持有，时间很短）
    if (xSemaphoreTake(s_buf_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      ESP_LOGW(TAG, "Failed to take mutex for index update");
      esp_camera_fb_return(fb);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // 快速更新缓冲区大小和索引
    if (next_buf_index == 0) {
      s_jpeg_size0 = jpeg_size;
    } else {
      s_jpeg_size1 = jpeg_size;
    }
    s_current_buf_index = next_buf_index;

    // 释放互斥锁
    xSemaphoreGive(s_buf_mutex);

    // 释放摄像头帧
    esp_camera_fb_return(fb);

    // 控制帧率（10 FPS）
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  ESP_LOGI(TAG, "Camera capture task stopped");
  vTaskDelete(NULL);
}

/* ================================================================
 * 私有函数：帧数据提供回调函数
 * ================================================================ */

/**
 * @brief 帧数据提供回调函数
 *
 * 向HTTP流媒体服务提供最新的JPEG帧数据。
 *
 * @param[out] jpeg_buf JPEG帧数据缓冲区指针
 * @param[out] jpeg_size JPEG帧数据大小
 * @return esp_err_t ESP_OK表示成功，其他值表示失败
 */
static esp_err_t s_frame_provider(uint8_t **jpeg_buf, size_t *jpeg_size) {
  // 获取互斥锁
  if (xSemaphoreTake(s_buf_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
    ESP_LOGW(TAG, "Failed to take mutex in frame provider");
    return ESP_ERR_TIMEOUT;
  }

  // 获取当前缓冲区
  uint8_t *buf = (s_current_buf_index == 0) ? s_jpeg_buf0 : s_jpeg_buf1;
  size_t size = (s_current_buf_index == 0) ? s_jpeg_size0 : s_jpeg_size1;

  // 检查帧数据是否有效
  if (size == 0) {
    ESP_LOGW(TAG, "No valid frame data available");
    xSemaphoreGive(s_buf_mutex);
    return ESP_ERR_NOT_FOUND;
  }

  // 返回帧数据
  *jpeg_buf = buf;
  *jpeg_size = size;

  // 释放互斥锁
  xSemaphoreGive(s_buf_mutex);

  return ESP_OK;
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

esp_err_t app_camera_stream_start(void) {
  // 检查是否已运行
  if (s_running) {
    ESP_LOGW(TAG, "Camera stream app already running");
    return ESP_OK;
  }

  // 初始化摄像头
  esp_err_t ret = bsp_camera_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize camera: %s", esp_err_to_name(ret));
    return ret;
  }

  // 创建互斥锁
  s_buf_mutex = xSemaphoreCreateMutex();
  if (s_buf_mutex == NULL) {
    ESP_LOGE(TAG, "Failed to create mutex");
    bsp_camera_deinit();
    return ESP_ERR_NO_MEM;
  }

  // 分配JPEG缓冲区（从PSRAM分配，避免占用内部SRAM）
  s_jpeg_buf0 =
      heap_caps_malloc(JPEG_BUF_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  s_jpeg_buf1 =
      heap_caps_malloc(JPEG_BUF_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (s_jpeg_buf0 == NULL || s_jpeg_buf1 == NULL) {
    ESP_LOGE(TAG, "Failed to allocate JPEG buffers from PSRAM");
    if (s_jpeg_buf0)
      free(s_jpeg_buf0);
    if (s_jpeg_buf1)
      free(s_jpeg_buf1);
    s_jpeg_buf0 = NULL;
    s_jpeg_buf1 = NULL;
    vSemaphoreDelete(s_buf_mutex);
    s_buf_mutex = NULL;
    bsp_camera_deinit();
    return ESP_ERR_NO_MEM;
  }
  ESP_LOGI(TAG, "JPEG buffers allocated from PSRAM: %zu bytes each",
           JPEG_BUF_SIZE);

  // 设置运行标志
  s_running = true;

  // 启动摄像头捕获任务
  BaseType_t task_ret =
      xTaskCreatePinnedToCore(s_camera_capture_task, "camera_capture", 4096,
                              NULL, 5, &s_capture_task_handle,
                              1); // Core 1

  if (task_ret != pdPASS) {
    ESP_LOGE(TAG, "Failed to create camera capture task");
    s_running = false;
    free(s_jpeg_buf0);
    free(s_jpeg_buf1);
    s_jpeg_buf0 = NULL;
    s_jpeg_buf1 = NULL;
    vSemaphoreDelete(s_buf_mutex);
    s_buf_mutex = NULL;
    bsp_camera_deinit();
    return ESP_ERR_NO_MEM;
  }

  // 启动HTTP流媒体服务，注入帧数据提供回调函数
  ret = http_stream_service_start(s_frame_provider);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start HTTP stream service: %s",
             esp_err_to_name(ret));
    s_running = false;
    vTaskDelete(s_capture_task_handle);
    s_capture_task_handle = NULL;
    free(s_jpeg_buf0);
    free(s_jpeg_buf1);
    s_jpeg_buf0 = NULL;
    s_jpeg_buf1 = NULL;
    vSemaphoreDelete(s_buf_mutex);
    s_buf_mutex = NULL;
    bsp_camera_deinit();
    return ret;
  }

  ESP_LOGI(TAG, "Camera stream app started successfully");
  return ESP_OK;
}

esp_err_t app_camera_stream_stop(void) {
  // 检查是否已停止
  if (!s_running) {
    ESP_LOGW(TAG, "Camera stream app already stopped");
    return ESP_OK;
  }

  // 设置停止标志
  s_running = false;

  // 停止HTTP流媒体服务
  esp_err_t ret = http_stream_service_stop();
  if (ret != ESP_OK) {
    ESP_LOGW(TAG, "Failed to stop HTTP stream service: %s",
             esp_err_to_name(ret));
  }

  // 等待摄像头捕获任务结束
  if (s_capture_task_handle != NULL) {
    vTaskDelay(pdMS_TO_TICKS(200));
    s_capture_task_handle = NULL;
  }

  // 删除互斥锁
  if (s_buf_mutex != NULL) {
    vSemaphoreDelete(s_buf_mutex);
    s_buf_mutex = NULL;
  }

  // 释放JPEG缓冲区
  if (s_jpeg_buf0 != NULL) {
    free(s_jpeg_buf0);
    s_jpeg_buf0 = NULL;
  }
  if (s_jpeg_buf1 != NULL) {
    free(s_jpeg_buf1);
    s_jpeg_buf1 = NULL;
  }
  s_jpeg_size0 = 0;
  s_jpeg_size1 = 0;

  // 释放摄像头资源
  ret = bsp_camera_deinit();
  if (ret != ESP_OK) {
    ESP_LOGW(TAG, "Failed to deinitialize camera: %s", esp_err_to_name(ret));
  }

  ESP_LOGI(TAG, "Camera stream app stopped successfully");
  return ESP_OK;
}

bool app_camera_stream_is_running(void) { return s_running; }