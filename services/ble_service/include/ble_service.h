#pragma once

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 NimBLE 蓝牙服务并创建基础 GATT 数据库结构
 */
void ble_service_init(void);

/**
 * @brief 开启 BLE GAP 广播，使外部设备（如手机）可发现并连接本设备
 * 
 * @return esp_err_t ESP_OK 表示成功开启广播
 */
esp_err_t ble_service_start(void);

/**
 * @brief 停止 BLE 广播与任何当前活动的蓝牙连接
 * 
 * @return esp_err_t ESP_OK 表示成功停止
 */
esp_err_t ble_service_stop(void);

/**
 * @brief 检查当前是否正在进行蓝牙广播
 * 
 * @return true 正在广播
 * @return false 未在广播
 */
bool ble_service_is_advertising(void);

#ifdef __cplusplus
}
#endif
