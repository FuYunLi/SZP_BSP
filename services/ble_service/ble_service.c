#include "ble_service.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* NimBLE 核心头文件 */
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

/* 外部函数声明：配对信息存储初始化 */
extern void ble_store_config_init(void);

static const char *TAG = "ble_service";
static uint8_t s_own_addr_type = 0;
static bool s_is_advertising = false;
static bool s_is_initialized = false;
static bool s_host_task_running = false;
static TaskHandle_t s_ble_host_task_handle = NULL;

// 蓝牙设备默认广播名称
#define BLE_DEVICE_NAME "LCSZP-S3-BLE"

/* GATT 访问回调：为未来屏幕及蓝牙二维码配网留下的数据传输接口 */
static int gatt_svr_chr_access_prov(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt,
                                    void *arg) {
  const ble_uuid_t *uuid = ctxt->chr->uuid;

  /* 检查对应的配置特征值 UUID 0xFFF1 */
  if (ble_uuid_cmp(uuid, BLE_UUID16_DECLARE(0xFFF1)) != 0) {
    return BLE_ATT_ERR_REQ_NOT_SUPPORTED;
  }

  if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
    uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
    uint8_t rx_buf[128] = {0};

    if (len >= sizeof(rx_buf)) {
      return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    int rc = ble_hs_mbuf_to_flat(ctxt->om, rx_buf, len, NULL);
    if (rc != 0) {
      return BLE_ATT_ERR_UNLIKELY;
    }

    ESP_LOGI(TAG, "Received BLE provisioning raw data (len=%d): %s", len,
             (char *)rx_buf);
    /*
     * 【预留配网接口点】：
     * 此处接收到来自手机端的 SSID 和 Password 后，可以直接调用：
     * wifi_service_connect((char*)ssid, (char*)password);
     */
    return 0;
  } else if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
    /* 回复当前连接状态或辅助信息 */
    const char *status_str = "READY_TO_PROVISION";
    int rc = os_mbuf_append(ctxt->om, status_str, strlen(status_str));
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }

  return BLE_ATT_ERR_REQ_NOT_SUPPORTED;
}

/* 自定义 GATT 数据库定义 */
static const struct ble_gatt_svc_def s_gatt_svr_svcs[] = {
    {
        /* 服务：配网辅助服务 */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0xFFF0),
        .characteristics =
            (struct ble_gatt_chr_def[]){
                {
                    /* 特征值：SSID/Password 写特征值与连接状态读特征值 */
                    .uuid = BLE_UUID16_DECLARE(0xFFF1),
                    .access_cb = gatt_svr_chr_access_prov,
                    .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE,
                },
                {
                    0, /* 结束特征值列表 */
                }},
    },
    {
        0, /* 结束服务列表 */
    }};

/* GAP 事件监听回调 */
static int ble_svc_gap_event(struct ble_gap_event *event, void *arg) {
  struct ble_gap_conn_desc desc;
  int rc;

  switch (event->type) {
  case BLE_GAP_EVENT_CONNECT:
    ESP_LOGI(TAG, "BLE connection established; status=%d ",
             event->connect.status);
    s_is_advertising = false;
    if (event->connect.status == 0) {
      rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
      if (rc == 0) {
        ESP_LOGI(TAG, "Connected to master device.");
      }
    }
    break;

  case BLE_GAP_EVENT_DISCONNECT:
    ESP_LOGI(TAG, "BLE connection disconnected; reason=%d ",
             event->disconnect.reason);
    s_is_advertising = false;
    break;

  case BLE_GAP_EVENT_ADV_COMPLETE:
    ESP_LOGI(TAG, "BLE advertising completed; reason=%d ",
             event->adv_complete.reason);
    s_is_advertising = false;
    break;

  case BLE_GAP_EVENT_ENC_CHANGE:
    /* 加密状态变化事件（配对成功后触发） */
    ESP_LOGI(TAG, "Encryption change event; status=%d",
             event->enc_change.status);
    break;

  case BLE_GAP_EVENT_PASSKEY_ACTION:
    /* 配对密钥输入事件 - 开发调试阶段自动接受 */
    ESP_LOGI(TAG, "Passkey action event, action=%d",
             event->passkey.params.action);
    {
      struct ble_sm_io pkey = {0};
      if (event->passkey.params.action == BLE_SM_IOACT_NUMCMP) {
        /* 数字比较：自动接受配对 */
        pkey.action = BLE_SM_IOACT_NUMCMP;
        pkey.numcmp_accept = 1;
        ESP_LOGI(TAG, "Auto-accepting pairing with passkey: %lu",
                 event->passkey.params.numcmp);
      } else if (event->passkey.params.action == BLE_SM_IOACT_OOB) {
        /* OOB配对：发送空数据（开发阶段） */
        pkey.action = BLE_SM_IOACT_OOB;
      } else if (event->passkey.params.action == BLE_SM_IOACT_INPUT) {
        /* 输入密钥：发送0（开发阶段） */
        pkey.action = BLE_SM_IOACT_INPUT;
        pkey.passkey = 0;
      } else if (event->passkey.params.action == BLE_SM_IOACT_DISP) {
        /* 显示密钥：返回生成的密钥 */
        pkey.action = BLE_SM_IOACT_DISP;
        pkey.passkey = event->passkey.params.numcmp;
      }
      ble_sm_inject_io(event->passkey.conn_handle, &pkey);
    }
    break;

  default:
    break;
  }
  return 0;
}

/* 开启 GAP 广播 */
static void ble_svc_advertise(void) {
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;
  int rc;

  memset(&fields, 0, sizeof(fields));

  /* 1. 配置广播标志和名字 */
  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
  fields.name = (uint8_t *)BLE_DEVICE_NAME;
  fields.name_len = strlen(BLE_DEVICE_NAME);
  fields.name_is_complete = 1;

  rc = ble_gap_adv_set_fields(&fields);
  if (rc != 0) {
    ESP_LOGE(TAG, "error setting advertisement data; rc=%d", rc);
    return;
  }

  /* 2. 配置 GAP 广播参数 */
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

  rc = ble_gap_adv_start(s_own_addr_type, NULL, BLE_HS_FOREVER, &adv_params,
                         ble_svc_gap_event, NULL);
  if (rc != 0) {
    ESP_LOGE(TAG, "error enabling advertisement; rc=%d", rc);
    return;
  }

  s_is_advertising = true;
  ESP_LOGI(TAG, "BLE advertising successfully started as '%s'",
           BLE_DEVICE_NAME);
}

/* NimBLE 主机线程回调 */
static void ble_host_task(void *param) {
  ESP_LOGI(TAG, "NimBLE Host Task Started.");
  s_host_task_running = true;

  /* 阻塞运行蓝牙协议栈的主循环，只有当调用 nimble_port_stop() 时本函数才会返回
   */
  nimble_port_run();

  ESP_LOGI(TAG, "NimBLE Host Task Stopped.");
  nimble_port_freertos_deinit();
  s_host_task_running = false;
  s_ble_host_task_handle = NULL;
  vTaskDelete(NULL);
}

/* NimBLE 同步回调 */
static void ble_svc_on_sync(void) {
  int rc = ble_hs_util_ensure_addr(0);
  if (rc != 0) {
    ESP_LOGE(TAG, "Failed to ensure address: %d", rc);
    return;
  }

  /* 获取本机的广播地址类型 */
  rc = ble_hs_id_infer_auto(0, &s_own_addr_type);
  if (rc != 0) {
    ESP_LOGE(TAG, "error determining address type; rc=%d", rc);
    return;
  }

  ESP_LOGI(TAG, "NimBLE stack sync completed. Stack is ready.");
}

/* 蓝牙服务初始化 */
void ble_service_init(void) {
  if (s_is_initialized) {
    return;
  }

  ESP_LOGI(TAG, "Initializing NimBLE Bluetooth Host Stack...");

  /* 1. 初始化 NimBLE 控制器与端口层 */
  int rc = nimble_port_init();
  if (rc != 0) {
    ESP_LOGE(TAG, "Failed to initialize nimble port: %d", rc);
    return;
  }

  /* 2. 配置 NimBLE 同步回调 */
  ble_hs_cfg.sync_cb = ble_svc_on_sync;

  /* 3. 配置安全管理器（开发调试阶段：无输入输出能力，简化配对） */
  ble_hs_cfg.sm_io_cap = BLE_HS_IO_NO_INPUT_OUTPUT; // 无输入输出能力
  ble_hs_cfg.sm_bonding = 1;                        // 启用绑定
  ble_hs_cfg.sm_mitm = 0;                           // 禁用MITM保护（开发阶段）
  ble_hs_cfg.sm_sc = 1;                             // 启用安全连接

  /* 初始化配对信息存储 */
  ble_store_config_init();

  /* 4. 初始化内置 GAP 与 GATT 服务，并注册我们的自定义配网服务数据库 */
  ble_svc_gap_init();
  ble_svc_gatt_init();

  rc = ble_gatts_count_cfg(s_gatt_svr_svcs);
  if (rc != 0) {
    ESP_LOGE(TAG, "Failed to count GATT config: %d", rc);
    return;
  }

  rc = ble_gatts_add_svcs(s_gatt_svr_svcs);
  if (rc != 0) {
    ESP_LOGE(TAG, "Failed to add GATT services: %d", rc);
    return;
  }

  /* 4. 配置 GAP 设备名称 */
  rc = ble_svc_gap_device_name_set(BLE_DEVICE_NAME);
  if (rc != 0) {
    ESP_LOGE(TAG, "Failed to set GAP device name: %d", rc);
    return;
  }

  s_is_initialized = true;
  ESP_LOGI(TAG, "NimBLE Bluetooth initialized successfully.");
}

/* 启动蓝牙广播 */
esp_err_t ble_service_start(void) {
  if (!s_is_initialized) {
    return ESP_ERR_INVALID_STATE;
  }

  if (s_is_advertising) {
    ESP_LOGW(TAG, "BLE is already advertising.");
    return ESP_OK;
  }

  /* 如果 Host 线程还没有起来，先创建 Host 线程来跑协议栈 */
  if (!s_host_task_running) {
    BaseType_t ret = xTaskCreatePinnedToCore(
        ble_host_task, "ble_host", 4096, NULL, 5, &s_ble_host_task_handle, 0);
    if (ret != pdPASS) {
      ESP_LOGE(TAG, "Failed to create BLE host task.");
      return ESP_FAIL;
    }

    // 等待几毫秒以确保 NimBLE 在广告开启前同步完毕
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  ble_svc_advertise();
  return ESP_OK;
}

/* 停止蓝牙广播 */
esp_err_t ble_service_stop(void) {
  if (!s_is_initialized) {
    return ESP_ERR_INVALID_STATE;
  }

  if (s_is_advertising) {
    int rc = ble_gap_adv_stop();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
      ESP_LOGE(TAG, "Failed to stop BLE advertisement: %d", rc);
      return ESP_FAIL;
    }
    s_is_advertising = false;
    ESP_LOGI(TAG, "BLE advertisement stopped.");
  }

  /*
   * 如果要完全关闭蓝牙协议栈释放内存，可以调用 nimble_port_stop()
   * 这里为保证调试指令可用，暂不删除主机任务，只关闭广播
   */
  return ESP_OK;
}

/* 检查是否在广播 */
bool ble_service_is_advertising(void) { return s_is_advertising; }
