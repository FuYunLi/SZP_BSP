#include "wifi_service.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

static const char *TAG = "wifi_service";
#define WIFI_STORE_NAMESPACE "wifi_store"
#define MAX_RETRY_CONN 5

/* 定义事件基 */
ESP_EVENT_DEFINE_BASE(WIFI_SERVICE_EVENT_BASE);

static esp_netif_t *s_sta_netif = NULL;
static esp_netif_t *s_ap_netif = NULL;
static wifi_service_state_t s_wifi_state = WIFI_SERVICE_STATE_DISCONNECTED;
static int s_retry_num = 0;
static bool s_is_initialized = false;
static bool s_sta_started = false;
static bool s_ap_started = false;

// 存储临时 SSID 和 PWD，用于断线重连
static char s_current_ssid[33] = {0};
static char s_current_password[65] = {0};

/* 更改内部状态，并向默认事件循环分发自定义事件 */
static void update_state(wifi_service_state_t new_state)
{
    s_wifi_state = new_state;
    wifi_service_event_id_t event_id;

    switch (new_state)
    {
        case WIFI_SERVICE_STATE_CONNECTING:
            event_id = WIFI_SERVICE_EVENT_CONNECTING;
            break;
        case WIFI_SERVICE_STATE_CONNECTED:
            event_id = WIFI_SERVICE_EVENT_CONNECTED;
            break;
        case WIFI_SERVICE_STATE_GOT_IP:
            event_id = WIFI_SERVICE_EVENT_GOT_IP;
            break;
        case WIFI_SERVICE_STATE_FAILED:
            event_id = WIFI_SERVICE_EVENT_FAILED;
            break;
        case WIFI_SERVICE_STATE_DISCONNECTED:
        default:
            event_id = WIFI_SERVICE_EVENT_DISCONNECTED;
            break;
    }

    ESP_LOGD(TAG, "Posting state change event: %d", event_id);
    esp_event_post(WIFI_SERVICE_EVENT_BASE, event_id, NULL, 0, portMAX_DELAY);
}

/* Wi-Fi & IP 事件接收回调 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "STA Mode Started. Attempting connection to SSID: %s", s_current_ssid);
        esp_wifi_connect();
        update_state(WIFI_SERVICE_STATE_CONNECTING);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconn = (wifi_event_sta_disconnected_t*) event_data;
        ESP_LOGW(TAG, "Disconnected from AP, reason code: %d", disconn->reason);
        
        if (s_retry_num < MAX_RETRY_CONN)
        {
            s_retry_num++;
            ESP_LOGI(TAG, "Reconnecting to AP (%d/%d)...", s_retry_num, MAX_RETRY_CONN);
            esp_wifi_connect();
            update_state(WIFI_SERVICE_STATE_CONNECTING);
        }
        else
        {
            ESP_LOGE(TAG, "Connection failed after %d retries.", MAX_RETRY_CONN);
            update_state(WIFI_SERVICE_STATE_FAILED);
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG, "Connected to AP, waiting for DHCP IP allocation...");
        s_retry_num = 0;
        update_state(WIFI_SERVICE_STATE_CONNECTED);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "DHCP Success! Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        
        // 成功获取 IP 后，说明配置是正确的，将其自动保存到 NVS 以做断电记忆
        wifi_service_save_config(s_current_ssid, s_current_password);
        
        update_state(WIFI_SERVICE_STATE_GOT_IP);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START)
    {
        ESP_LOGI(TAG, "SoftAP started.");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STOP)
    {
        ESP_LOGI(TAG, "SoftAP stopped.");
    }
}

/* 初始化 Wi-Fi 协议栈 */
void wifi_service_init(void)
{
    if (s_is_initialized)
    {
        return;
    }

    ESP_LOGI(TAG, "Initializing Wi-Fi service...");

    // 1. 初始化底层的 TCP/IP 网络堆栈
    esp_err_t err = esp_netif_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize netif: %s", esp_err_to_name(err));
        return;
    }

    // 2. 创建默认的 STA 和 AP 的 Netif 网卡描述句柄
    s_sta_netif = esp_netif_create_default_wifi_sta();
    s_ap_netif = esp_netif_create_default_wifi_ap();

    // 3. 初始化 Wi-Fi 硬件与协议栈
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 4. 注册 Wi-Fi 事件监听器到全局的 Default Event Loop
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    // 5. 设置 Wi-Fi 存储介质为 RAM (我们将手动持久化到 NVS)
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    s_is_initialized = true;
    update_state(WIFI_SERVICE_STATE_DISCONNECTED);
    ESP_LOGI(TAG, "Wi-Fi service initialized successfully.");
}

/* 连接 Wi-Fi AP */
esp_err_t wifi_service_connect(const char *ssid, const char *password)
{
    if (!s_is_initialized)
    {
        ESP_LOGE(TAG, "Wi-Fi service not initialized yet!");
        return ESP_ERR_INVALID_STATE;
    }

    if (ssid == NULL || strlen(ssid) == 0)
    {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Connecting to SSID: %s", ssid);

    // 停止 AP 模式以防止干扰 (如果有)
    if (s_ap_started)
    {
        wifi_service_ap_stop();
    }

    // 如果目前已经是连上的，先断开
    if (s_sta_started)
    {
        esp_wifi_stop();
        s_sta_started = false;
    }

    // 暂存当前的凭证
    strncpy(s_current_ssid, ssid, sizeof(s_current_ssid) - 1);
    s_current_ssid[sizeof(s_current_ssid) - 1] = '\0';

    if (password != NULL)
    {
        strncpy(s_current_password, password, sizeof(s_current_password) - 1);
        s_current_password[sizeof(s_current_password) - 1] = '\0';
    }
    else
    {
        s_current_password[0] = '\0';
    }

    s_retry_num = 0;

    // 配置 STA 属性
    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, s_current_ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, s_current_password, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = password ? WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    // 启动 Wi-Fi 开始触发事件总线
    esp_err_t ret = esp_wifi_start();
    if (ret == ESP_OK)
    {
        s_sta_started = true;
    }
    return ret;
}

/* 加载 NVS 中存储的配置并尝试重连 */
esp_err_t wifi_service_connect_saved(void)
{
    char saved_ssid[33] = {0};
    char saved_password[65] = {0};

    esp_err_t err = wifi_service_load_config(saved_ssid, saved_password, sizeof(saved_ssid), sizeof(saved_password));
    if (err == ESP_OK && strlen(saved_ssid) > 0)
    {
        ESP_LOGI(TAG, "Found saved Wi-Fi configuration in NVS. Auto-reconnecting...");
        return wifi_service_connect(saved_ssid, saved_password);
    }
    else
    {
        ESP_LOGI(TAG, "No Wi-Fi configuration found in NVS.");
        return ESP_ERR_NOT_FOUND;
    }
}

/* 主动断开连接 */
esp_err_t wifi_service_disconnect(void)
{
    if (!s_sta_started)
    {
        return ESP_OK;
    }
    ESP_LOGI(TAG, "Disconnecting from Wi-Fi...");
    s_retry_num = MAX_RETRY_CONN; // 阻止自动重连
    esp_err_t err = esp_wifi_disconnect();
    esp_wifi_stop();
    s_sta_started = false;
    update_state(WIFI_SERVICE_STATE_DISCONNECTED);
    return err;
}

/* 开启 SoftAP 模式 */
esp_err_t wifi_service_ap_start(const char *ssid, const char *password)
{
    if (!s_is_initialized)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (ssid == NULL || strlen(ssid) == 0)
    {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Starting SoftAP. SSID: %s", ssid);

    // 断开 STA 模式以保证单模式干净运行
    if (s_sta_started)
    {
        wifi_service_disconnect();
    }

    if (s_ap_started)
    {
        esp_wifi_stop();
        s_ap_started = false;
    }

    wifi_config_t wifi_config = {
        .ap = {
            .max_connection = 4,
            .beacon_interval = 100,
            .channel = 1,
        },
    };

    strncpy((char *)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid) - 1);
    wifi_config.ap.ssid_len = strlen(ssid);

    if (password != NULL && strlen(password) >= 8)
    {
        strncpy((char *)wifi_config.ap.password, password, sizeof(wifi_config.ap.password) - 1);
        wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    }
    else
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    
    esp_err_t ret = esp_wifi_start();
    if (ret == ESP_OK)
    {
        s_ap_started = true;
    }
    return ret;
}

/* 关闭 SoftAP 模式 */
esp_err_t wifi_service_ap_stop(void)
{
    if (!s_ap_started)
    {
        return ESP_OK;
    }
    ESP_LOGI(TAG, "Stopping SoftAP...");
    esp_err_t err = esp_wifi_stop();
    s_ap_started = false;
    return err;
}

/* 获取状态 */
wifi_service_state_t wifi_service_get_state(void)
{
    return s_wifi_state;
}

/* 将凭证保存到 NVS */
esp_err_t wifi_service_save_config(const char *ssid, const char *password)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(WIFI_STORE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS namespace: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_set_str(my_handle, "ssid", ssid);
    if (err == ESP_OK)
    {
        err = nvs_set_str(my_handle, "password", password ? password : "");
    }

    if (err == ESP_OK)
    {
        err = nvs_commit(my_handle);
        ESP_LOGI(TAG, "Wi-Fi configuration successfully committed to NVS.");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to write Wi-Fi configuration to NVS.");
    }

    nvs_close(my_handle);
    return err;
}

/* 从 NVS 读取凭证 */
esp_err_t wifi_service_load_config(char *ssid, char *password, size_t max_ssid_len, size_t max_pwd_len)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(WIFI_STORE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err != ESP_OK)
    {
        return err;
    }

    err = nvs_get_str(my_handle, "ssid", ssid, &max_ssid_len);
    if (err == ESP_OK)
    {
        err = nvs_get_str(my_handle, "password", password, &max_pwd_len);
    }

    nvs_close(my_handle);
    return err;
}

/* 清除 NVS 配置 */
esp_err_t wifi_service_clear_config(void)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(WIFI_STORE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        return err;
    }

    err = nvs_erase_all(my_handle);
    if (err == ESP_OK)
    {
        err = nvs_commit(my_handle);
        ESP_LOGI(TAG, "Wi-Fi configuration cleared in NVS.");
    }
    nvs_close(my_handle);
    return err;
}
