#pragma once

#include "esp_err.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载 DVP 摄像头 (GC0308)
 * 
 * 包含调用 PCA9557 拉低 DVP_PWDN 引脚进行硬件唤醒，并调用 esp_camera_init 进行驱动初始化。
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_camera_init(void);

/**
 * @brief 去初始化板载 DVP 摄像头并将其置于休眠模式
 * 
 * 调用 esp_camera_deinit 并将 PCA9557 DVP_PWDN 引脚拉高以降低功耗。
 * 
 * @return esp_err_t ESP_OK 表示成功，其它值表示失败
 */
esp_err_t bsp_camera_deinit(void);

/**
 * @brief 捕获单帧图像数据
 * 
 * @return camera_fb_t* 指向捕获帧数据的指针，失败时返回 NULL
 */
camera_fb_t* bsp_camera_fb_get(void);

/**
 * @brief 释放捕获到的帧缓冲区
 * 
 * @param fb 指向需要释放的帧数据结构的指针
 * @return esp_err_t ESP_OK 表示成功
 */
esp_err_t bsp_camera_fb_return(camera_fb_t *fb);

#ifdef __cplusplus
}
#endif
