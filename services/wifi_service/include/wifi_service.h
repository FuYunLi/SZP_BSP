#pragma once

#include "esp_err.h"
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 声明 Wi-Fi 服务自定义事件基 */
ESP_EVENT_DECLARE_BASE(WIFI_SERVICE_EVENT_BASE);

/* Wi-Fi 服务状态变化事件 ID，供系统事件循环订阅使用 */
typedef enum {
    WIFI_SERVICE_EVENT_CONNECTING,
    WIFI_SERVICE_EVENT_CONNECTED,
    WIFI_SERVICE_EVENT_GOT_IP,
    WIFI_SERVICE_EVENT_DISCONNECTED,
    WIFI_SERVICE_EVENT_FAILED,
} wifi_service_event_id_t;

/* Wi-Fi 服务内部状态枚举 */
typedef enum {
    WIFI_SERVICE_STATE_DISCONNECTED,
    WIFI_SERVICE_STATE_CONNECTING,
    WIFI_SERVICE_STATE_CONNECTED,
    WIFI_SERVICE_STATE_GOT_IP,
    WIFI_SERVICE_STATE_FAILED,
} wifi_service_state_t;

/**
 * @brief 初始化 Wi-Fi 协议栈及核心配置
 */
void wifi_service_init(void);

/**
 * @brief 连接指定的 Wi-Fi AP（成功连接后会自动将配置写入 NVS）
 * 
 * @param ssid 目标 Wi-Fi SSID
 * @param password 目标 Wi-Fi 密码
 * @return esp_err_t ESP_OK 表示开始尝试连接，错误码表示初始化或启动失败
 */
esp_err_t wifi_service_connect(const char *ssid, const char *password);

/**
 * @brief 从 NVS 加载存储的 Wi-Fi SSID/Password 并尝试连接
 * 
 * @return esp_err_t ESP_OK 表示连接启动成功或无配置，错误码表示连接失败
 */
esp_err_t wifi_service_connect_saved(void);

/**
 * @brief 主动断开当前 Wi-Fi 连接
 * 
 * @return esp_err_t ESP_OK 表示断开成功
 */
esp_err_t wifi_service_disconnect(void);

/**
 * @brief 开启本地 SoftAP 热点服务
 * 
 * @param ssid 热点名称
 * @param password 热点密码 (为 NULL 或长度小于 8 表示开放热点)
 * @return esp_err_t ESP_OK 表示启动成功
 */
esp_err_t wifi_service_ap_start(const char *ssid, const char *password);

/**
 * @brief 关闭 SoftAP 热点服务
 * 
 * @return esp_err_t ESP_OK 表示关闭成功
 */
esp_err_t wifi_service_ap_stop(void);

/**
 * @brief 获取当前 Wi-Fi 服务连接状态
 * 
 * @return wifi_service_state_t 当前连接状态
 */
wifi_service_state_t wifi_service_get_state(void);

/**
 * @brief 手动将 SSID/Password 保存至 NVS
 */
esp_err_t wifi_service_save_config(const char *ssid, const char *password);

/**
 * @brief 手动从 NVS 加载 SSID/Password
 */
esp_err_t wifi_service_load_config(char *ssid, char *password, size_t max_ssid_len, size_t max_pwd_len);

/**
 * @brief 清除 NVS 中的 Wi-Fi 配置
 */
esp_err_t wifi_service_clear_config(void);

#ifdef __cplusplus
}
#endif
