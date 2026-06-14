#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "pca9557.h"
#include "bsp_camera.h"

static const char *TAG = "bsp_camera";

#define CAM_PIN_PWDN     -1
#define CAM_PIN_RESET    -1
#define CAM_PIN_XCLK     5
#define CAM_PIN_SIOD     1
#define CAM_PIN_SIOC     2

#define CAM_PIN_D7       9
#define CAM_PIN_D6       4
#define CAM_PIN_D5       6
#define CAM_PIN_D4       15
#define CAM_PIN_D3       17
#define CAM_PIN_D2       8
#define CAM_PIN_D1       18
#define CAM_PIN_D0       16
#define CAM_PIN_VSYNC    3
#define CAM_PIN_HREF     46
#define CAM_PIN_PCLK     7

#define PCA9557_IO_PWDN  2

esp_err_t bsp_camera_init(void)
{
    ESP_LOGI(TAG, "Initializing camera PWDN control via PCA9557...");
    
    // 1. 设置 PCA9557 IO2 为输出，并拉低（拉低表示唤醒摄像头，拉高表示休眠）
    esp_err_t err = pca9557_set_config(PCA9557_IO_PWDN, false);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set PCA9557 IO2 config: %s", esp_err_to_name(err));
        return err;
    }
    
    err = pca9557_set_output_level(PCA9557_IO_PWDN, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write PCA9557 IO2 output level: %s", esp_err_to_name(err));
        return err;
    }
    
    // 给 50ms 延时以允许摄像头内部供电稳定拉起
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // 2. 填充 esp32-camera 初始化结构体
    camera_config_t config = {
        .pin_pwdn = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        // SCCB/I2C: 设为 -1 以指示 esp32-camera 复用 BSP 已初始化的 I2C1 总线句柄
        // 通过 sccb_i2c_port=1 告知 sccb-ng 层调用 SCCB_Use_Port(1) 而非 i2c_new_master_bus()
        // 这样可以与 PCA9557、ES8311、ES7210、触摸屏和谐共存于同一 I2C 总线上
        .pin_sscb_sda = -1,
        .pin_sscb_scl = -1,
        .sccb_i2c_port = 1,
        
        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,
        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,
        
        // S3 在 20MHz 下能工作得很好，通过 LCD_CAM 传输
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        
        .pixel_format = PIXFORMAT_RGB565,  // 直出 16位 RGB565 像素便于处理
        .frame_size = FRAMESIZE_QVGA,       // 分辨率设置为 320x240 (QVGA)
        .jpeg_quality = 12,
        .fb_count = 1,                      // 对测试与单帧拍照，单缓冲区足矣
        .fb_location = CAMERA_FB_IN_PSRAM,  // 显式保存在 PSRAM 中，保证 SRAM 空间不被吃光
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };
    
    // 3. 执行摄像头硬件探测与初始化
    ESP_LOGI(TAG, "Calling esp_camera_init...");
    err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_camera_init failed: %s", esp_err_to_name(err));
        // 初始化失败时，重新将摄像头拉入休眠以节约功耗
        pca9557_set_output_level(PCA9557_IO_PWDN, 1);
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
    return ESP_OK;
}

esp_err_t bsp_camera_deinit(void)
{
    ESP_LOGI(TAG, "Deinitializing camera...");
    
    esp_err_t err = esp_camera_deinit();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_camera_deinit failed: %s", esp_err_to_name(err));
    }
    
    // 将 DVP_PWDN 引脚拉高，将摄像头强制送入休眠，降低待机电流功耗
    err = pca9557_set_output_level(PCA9557_IO_PWDN, 1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to pull DVP_PWDN high: %s", esp_err_to_name(err));
    }
    
    ESP_LOGI(TAG, "Camera deinitialized and set to standby mode.");
    return err;
}

camera_fb_t* bsp_camera_fb_get(void)
{
    return esp_camera_fb_get();
}

esp_err_t bsp_camera_fb_return(camera_fb_t *fb)
{
    esp_camera_fb_return(fb);
    return ESP_OK;
}
