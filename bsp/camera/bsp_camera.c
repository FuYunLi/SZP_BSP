#include "bsp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pca9557.h"
#include <stdio.h>

// 引脚配置从 Kconfig 读取，支持用户通过 menuconfig 自定义
#ifdef CONFIG_BSP_CAM_PIN_XCLK
#define PIN_NUM_CAM_XCLK    CONFIG_BSP_CAM_PIN_XCLK
#define PIN_NUM_CAM_VSYNC   CONFIG_BSP_CAM_PIN_VSYNC
#define PIN_NUM_CAM_HREF    CONFIG_BSP_CAM_PIN_HREF
#define PIN_NUM_CAM_PCLK    CONFIG_BSP_CAM_PIN_PCLK
#define PIN_NUM_CAM_D0      CONFIG_BSP_CAM_PIN_D0
#define PIN_NUM_CAM_D1      CONFIG_BSP_CAM_PIN_D1
#define PIN_NUM_CAM_D2      CONFIG_BSP_CAM_PIN_D2
#define PIN_NUM_CAM_D3      CONFIG_BSP_CAM_PIN_D3
#define PIN_NUM_CAM_D4      CONFIG_BSP_CAM_PIN_D4
#define PIN_NUM_CAM_D5      CONFIG_BSP_CAM_PIN_D5
#define PIN_NUM_CAM_D6      CONFIG_BSP_CAM_PIN_D6
#define PIN_NUM_CAM_D7      CONFIG_BSP_CAM_PIN_D7
#define PCA9557_IO_CAM_PWDN CONFIG_BSP_CAM_PCA9557_IO_PWDN
#else
// 默认值（立创实战派 S3 开发板 GC0308 DVP 接口）
#define PIN_NUM_CAM_XCLK    5
#define PIN_NUM_CAM_VSYNC   3
#define PIN_NUM_CAM_HREF    46
#define PIN_NUM_CAM_PCLK    7
#define PIN_NUM_CAM_D0      16
#define PIN_NUM_CAM_D1      18
#define PIN_NUM_CAM_D2      8
#define PIN_NUM_CAM_D3      17
#define PIN_NUM_CAM_D4      15
#define PIN_NUM_CAM_D5      6
#define PIN_NUM_CAM_D6      4
#define PIN_NUM_CAM_D7      9
#define PCA9557_IO_CAM_PWDN 2
#endif

// PWDN 和 RESET 引脚不直接连接 GPIO，由系统或 PCA9557 控制
#define PIN_NUM_CAM_PWDN    -1
#define PIN_NUM_CAM_RESET   -1

static const char *TAG = "bsp_camera";

// 摄像头初始化状态标志（避免重复初始化）
static bool s_camera_initialized = false;

esp_err_t bsp_camera_init(void) {
  // 检查是否已初始化，避免重复初始化导致崩溃
  if (s_camera_initialized) {
    ESP_LOGW(TAG, "Camera already initialized, skipping...");
    return ESP_OK;
  }

  ESP_LOGI(TAG, "Initializing camera PWDN control via PCA9557...");

  // 1. 设置 PCA9557 IO 为输出，并拉低（拉低表示唤醒摄像头，拉高表示休眠）
  esp_err_t err = pca9557_set_config(PCA9557_IO_CAM_PWDN, false);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to set PCA9557 IO config: %s", esp_err_to_name(err));
    return err;
  }

  err = pca9557_set_output_level(PCA9557_IO_CAM_PWDN, 0);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to write PCA9557 IO output level: %s",
             esp_err_to_name(err));
    return err;
  }

  // 给 50ms 延时以允许摄像头内部供电稳定拉起
  vTaskDelay(pdMS_TO_TICKS(50));

  // 2. 填充 esp32-camera 初始化结构体
  camera_config_t config = {
      .pin_pwdn = PIN_NUM_CAM_PWDN,
      .pin_reset = PIN_NUM_CAM_RESET,
      .pin_xclk = PIN_NUM_CAM_XCLK,
      // SCCB/I2C: 设为 -1 以指示 esp32-camera 复用 BSP 已初始化的 I2C1 总线句柄
      // 通过 sccb_i2c_port=1 告知 sccb-ng 层调用 SCCB_Use_Port(1) 而非
      // i2c_new_master_bus() 这样可以与
      // PCA9557、ES8311、ES7210、触摸屏和谐共存于同一 I2C 总线上
      .pin_sscb_sda = -1,
      .pin_sscb_scl = -1,
      .sccb_i2c_port = 1,

      .pin_d7 = PIN_NUM_CAM_D7,
      .pin_d6 = PIN_NUM_CAM_D6,
      .pin_d5 = PIN_NUM_CAM_D5,
      .pin_d4 = PIN_NUM_CAM_D4,
      .pin_d3 = PIN_NUM_CAM_D3,
      .pin_d2 = PIN_NUM_CAM_D2,
      .pin_d1 = PIN_NUM_CAM_D1,
      .pin_d0 = PIN_NUM_CAM_D0,
      .pin_vsync = PIN_NUM_CAM_VSYNC,
      .pin_href = PIN_NUM_CAM_HREF,
      .pin_pclk = PIN_NUM_CAM_PCLK,

      // S3 在 20MHz 下能工作得很好，通过 LCD_CAM 传输
      .xclk_freq_hz = 20000000,
      .ledc_timer = LEDC_TIMER_0,
      .ledc_channel = LEDC_CHANNEL_0,

      // GC0308支持的格式：YUV422, RGB565, RAW Bayer, Grayscale
      // 注意：GC0308不支持JPEG压缩格式（硬件限制）
      .pixel_format = PIXFORMAT_RGB565,  // 直出 16位 RGB565 像素便于处理
      .frame_size = FRAMESIZE_QVGA,      // 分辨率设置为 320x240 (QVGA)
      .jpeg_quality = 12,                // 此参数对非JPEG格式无效
      .fb_count = 1,                     // 对测试与单帧拍照，单缓冲区足矣
      .fb_location = CAMERA_FB_IN_PSRAM, // 帧缓冲区显式保存在 PSRAM 中
      .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
  };

  // 3. 执行摄像头硬件探测与初始化
  ESP_LOGI(TAG, "Calling esp_camera_init...");
  err = esp_camera_init(&config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "esp_camera_init failed: %s", esp_err_to_name(err));
    // 初始化失败时，重新将摄像头拉入休眠以节约功耗
    pca9557_set_output_level(PCA9557_IO_CAM_PWDN, 1);
    return err;
  }

  // 获取传感器并微调参数（GC0308 默认需要取消水平镜像，使之成像方向正确）
  sensor_t *s = esp_camera_sensor_get();
  if (s != NULL) {
    ESP_LOGI(TAG, "Detected camera sensor PID: 0x%02X", s->id.PID);
    if (s->id.PID == GC0308_PID) {
      s->set_hmirror(s, 0); // 取消水平镜像，恢复正向画面
    }
  }

  ESP_LOGI(TAG, "Camera initialized successfully.");

  // 标记初始化状态为已完成
  s_camera_initialized = true;

  return ESP_OK;
}

esp_err_t bsp_camera_deinit(void) {
  // 检查是否已初始化，避免重复去初始化
  if (!s_camera_initialized) {
    ESP_LOGW(TAG, "Camera not initialized, skipping deinit...");
    return ESP_OK;
  }

  ESP_LOGI(TAG, "Deinitializing camera...");

  esp_err_t err = esp_camera_deinit();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "esp_camera_deinit failed: %s", esp_err_to_name(err));
  }

  // 将 DVP_PWDN 引脚拉高，将摄像头强制送入休眠，降低待机电流功耗
  err = pca9557_set_output_level(PCA9557_IO_CAM_PWDN, 1);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to pull DVP_PWDN high: %s", esp_err_to_name(err));
  }

  // 清除初始化状态标志
  s_camera_initialized = false;

  ESP_LOGI(TAG, "Camera deinitialized and set to standby mode.");
  return err;
}

camera_fb_t *bsp_camera_fb_get(void) { return esp_camera_fb_get(); }

esp_err_t bsp_camera_fb_return(camera_fb_t *fb) {
  esp_camera_fb_return(fb);
  return ESP_OK;
}
