/**
 * @file app_ui.c
 * @brief 应用层 UI 业务逻辑源文件
 */

#include "app_ui.h"
#include "lvgl.h"
#include "bsp_lvgl_port.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"

static const char *TAG = "app_ui";

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

    // 2. 锁定 LVGL 互斥锁以安全创建和配置 UI 控件 (多任务防冲突保护)
    // 0 表示无限等待直到获取到锁
    if (lvgl_port_lock(0))
    {
        ESP_LOGI(TAG, "成功获取 UI 互斥锁，正在构建测试界面...");

        // 创建一个简单的测试按钮
        lv_obj_t *btn = lv_button_create(lv_screen_active());
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40);
        
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, "Click Me!");

        // 创建一个简单的滑块
        lv_obj_t *slider = lv_slider_create(lv_screen_active());
        lv_obj_align(slider, LV_ALIGN_CENTER, 0, 40);

        // 创建完毕，解锁释放给 UI 运行引擎
        lvgl_port_unlock();
        
        ESP_LOGI(TAG, "应用层 UI 测试页面绘制完毕并成功解锁");
    }
    else
    {
        ESP_LOGE(TAG, "获取 LVGL 互斥锁超时或失败");
    }
}
