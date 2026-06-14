/**
 * @file app_ui.c
 * @brief 应用层 UI 业务逻辑源文件
 */

#include "app_ui.h"
#include "lvgl.h"
#include "demos/lv_demos.h"
#include "bsp_lvgl_port.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "app_ui";

static char s_next_demo_name[32] = {0};

/**
 * @brief UI 切换的异步回调函数，将在 UI 线程 (taskLVGL) 上下文中被安全执行
 */
static void async_show_demo_cb(void *user_data)
{
    const char *demo_name = (const char *)user_data;
    ESP_LOGI(TAG, "[Async Callback] 开始执行 UI 切换，目标 Demo: %s", demo_name);

    // 1. 清空当前活动屏幕上的所有子对象 (在 UI 线程内执行绝对安全)
    lv_obj_clean(lv_screen_active());

    // 2. 根据名称加载 demo
    if (strcmp(demo_name, "widgets") == 0)
    {
#if CONFIG_LV_USE_DEMO_WIDGETS
        lv_demo_widgets();
        ESP_LOGI(TAG, "LVGL Widgets Demo 已成功启动");
#else
        ESP_LOGW(TAG, "LV_USE_DEMO_WIDGETS 未在 sdkconfig 中启用！");
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Widgets Demo not enabled in sdkconfig");
        lv_obj_center(label);
#endif
    }
    else if (strcmp(demo_name, "benchmark") == 0)
    {
#if CONFIG_LV_USE_DEMO_BENCHMARK
        lv_demo_benchmark();
        ESP_LOGI(TAG, "LVGL Benchmark Demo 已成功启动");
#else
        ESP_LOGW(TAG, "LV_USE_DEMO_BENCHMARK 未在 sdkconfig 中启用！");
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Benchmark Demo not enabled in sdkconfig");
        lv_obj_center(label);
#endif
    }
    else if (strcmp(demo_name, "music") == 0)
    {
#if CONFIG_LV_USE_DEMO_MUSIC
        lv_demo_music();
        ESP_LOGI(TAG, "LVGL Music Demo 已成功启动");
#else
        ESP_LOGW(TAG, "LV_USE_DEMO_MUSIC 未在 sdkconfig 中启用！");
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Music Demo not enabled in sdkconfig");
        lv_obj_center(label);
#endif
    }
    else if (strcmp(demo_name, "stress") == 0)
    {
#if CONFIG_LV_USE_DEMO_STRESS
        lv_demo_stress();
        ESP_LOGI(TAG, "LVGL Stress Demo 已成功启动");
#else
        ESP_LOGW(TAG, "LV_USE_DEMO_STRESS 未在 sdkconfig 中启用！");
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Stress Demo not enabled in sdkconfig");
        lv_obj_center(label);
#endif
    }
    else
    {
        // 默认测试 Demo
        ESP_LOGI(TAG, "正在绘制默认的显示和触摸测试页面...");

        // 创建一个简单的测试按钮
        lv_obj_t *btn = lv_button_create(lv_screen_active());
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40);
        
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, "Click Me!");

        // 创建一个简单的滑块
        lv_obj_t *slider = lv_slider_create(lv_screen_active());
        lv_obj_align(slider, LV_ALIGN_CENTER, 0, 40);
    }
    ESP_LOGI(TAG, "[Async Callback] UI 切换完成，目标: %s", demo_name);
}

void app_ui_show_demo(const char *demo_name)
{
    // 1. 锁定 LVGL 互斥锁以安全写入异步队列 (多任务防冲突保护)
    if (lvgl_port_lock(0))
    {
        // 2. 将目标 Demo 名称复制到静态缓冲区以确保异步回调时生命周期有效
        strncpy(s_next_demo_name, demo_name, sizeof(s_next_demo_name) - 1);
        s_next_demo_name[sizeof(s_next_demo_name) - 1] = '\0';

        // 3. 注册异步操作，等待 UI 任务在其周期中执行
        lv_async_call(async_show_demo_cb, s_next_demo_name);

        // 4. 立即解锁释放互斥锁，返回控制台任务
        lvgl_port_unlock();
        ESP_LOGI(TAG, "UI 切换指令 '%s' 已成功排入异步队列", demo_name);
    }
    else
    {
        ESP_LOGE(TAG, "获取 LVGL 互斥锁超时，切换 Demo 失败");
    }
}

void app_ui_start(void)
{
    ESP_LOGI(TAG, "正在启动应用层 UI 初始化...");

    // 1. 初始化板级 LVGL 移植层 (自动对接硬件、启动定时器并创建独立的 UI 运行任务)
    esp_err_t err = bsp_lvgl_port_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 LVGL 移植层失败: %s", esp_err_to_name(err));
        return;
    }

    // 2. 显示默认界面
    app_ui_show_demo("default");
}
