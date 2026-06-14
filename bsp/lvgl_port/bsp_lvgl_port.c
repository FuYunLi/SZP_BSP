/**
 * @file bsp_lvgl_port.c
 * @brief LVGL 9.x 乐鑫官方组件适配移植层实现源文件
 */

#include "bsp_lvgl_port.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"

static const char *TAG = "bsp_lvgl_port";
static lv_display_t *s_disp = NULL;
static bool s_port_initialized = false;

esp_err_t bsp_lvgl_port_init(void)
{
    if (s_port_initialized)
    {
        ESP_LOGW(TAG, "LVGL 移植层已初始化");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "正在初始化乐鑫官方 esp_lvgl_port 框架...");

    // 1. 初始化 LVGL 接口框架与任务机制（内部使用高精度定时器并创建运行 lv_timer_handler 的 FreeRTOS 任务）
    const lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    esp_err_t err = lvgl_port_init(&port_cfg);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "初始化 esp_lvgl_port 失败: %s", esp_err_to_name(err));
        return err;
    }

    // 2. 获取 LCD 设备句柄
    esp_lcd_panel_handle_t panel = bsp_lcd_get_panel_handle();
    esp_lcd_panel_io_handle_t io = bsp_lcd_get_io_handle();
    if (panel == NULL || io == NULL)
    {
        ESP_LOGE(TAG, "获取 LCD 句柄失败，请确保在初始化 LVGL 前已调用 bsp_lcd_init");
        return ESP_ERR_INVALID_STATE;
    }

    // 3. 配置显示端口参数并添加显示屏
    // 【性能优化 1：内部 SRAM 极速双缓冲】
    // 【性能优化 2：底层绑定 on_color_trans_done 回调，实现异步零忙等刷新】
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io,
        .panel_handle = panel,
        .buffer_size = LCD_H_RES * 24, // 设置缓冲区深度为 24 行像素 (1/10 屏幕高度)
        .double_buffer = true,        // 启用乒乓双缓冲提高并发帧率
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .flags = {
            .buff_dma = true,         // 使用 DMA 缓冲
            .buff_spiram = false,     // 禁用外部 PSRAM，强制存放在内部 SRAM (Zero Cache Miss)
        }
    };

    s_disp = lvgl_port_add_disp(&disp_cfg);
    if (s_disp == NULL)
    {
        ESP_LOGE(TAG, "向 esp_lvgl_port 添加显示器失败");
        return ESP_FAIL;
    }

    // 4. 获取电容触摸屏驱动句柄并添加输入设备
    esp_lcd_touch_handle_t touch_handle = bsp_touch_get_handle();
    if (touch_handle != NULL)
    {
        ESP_LOGI(TAG, "检测到触摸驱动，正在注册触摸输入设备...");
        const lvgl_port_touch_cfg_t touch_cfg = {
            .disp = s_disp,
            .handle = touch_handle,
        };
        lv_indev_t *indev = lvgl_port_add_touch(&touch_cfg);
        if (indev == NULL)
        {
            ESP_LOGW(TAG, "添加触摸输入设备失败，触控可能无法使用");
        }
    }
    else
    {
        ESP_LOGW(TAG, "未检测到有效的触摸驱动句柄，跳过触摸注册");
    }

    s_port_initialized = true;
    ESP_LOGI(TAG, "LVGL 移植层初始化并对接成功");
    return ESP_OK;
}

lv_display_t *bsp_lvgl_port_get_display(void)
{
    return s_disp;
}
