#include "bsp_touch.h"
#include "bsp_i2c.h"
#include "esp_log.h"
#include "esp_lcd_touch_ft5x06.h"

static const char *TAG = "bsp_touch";
static esp_lcd_touch_handle_t s_touch_handle = NULL;

esp_err_t bsp_touch_init(void)
{
    if (s_touch_handle != NULL)
    {
        ESP_LOGW(TAG, "Touch driver already initialized");
        return ESP_OK;
    }

    i2c_master_bus_handle_t i2c_bus = bsp_i2c_get_bus_handle();
    if (i2c_bus == NULL)
    {
        ESP_LOGE(TAG, "I2C1 bus is not initialized, please initialize bsp_i2c first");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Initializing FT5x06 touch driver...");

    // 1. 配置 I2C 面板 IO 属性
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();
    
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_err_t ret = esp_lcd_new_panel_io_i2c(i2c_bus, &tp_io_config, &tp_io_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create touch Panel IO: %s", esp_err_to_name(ret));
        return ret;
    }

    // 2. 配置触摸驱动参数 (240x320 物理基础，结合 swap/mirror 校准输出)
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = 240, 
        .y_max = 320,
        .rst_gpio_num = -1, // 未分配 RST GPIO
        .int_gpio_num = -1, // 未分配 INT GPIO
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 1,
            .mirror_x = 1,
            .mirror_y = 0,
        },
    };

    // 3. 实例化 FT5x06 驱动
    ret = esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, &s_touch_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to instantiate FT5x06 touch handle: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "FT5x06 touch driver initialized successfully");
    return ESP_OK;
}

esp_err_t bsp_touch_read(uint16_t *x, uint16_t *y, uint8_t *points_num)
{
    if (s_touch_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    // 更新内部数据缓冲区 (轮询读取 I2C 寄存器)
    esp_err_t ret = esp_lcd_touch_read_data(s_touch_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }

    uint16_t touch_x[1] = {0};
    uint16_t touch_y[1] = {0};
    uint16_t strength[1] = {0};
    uint8_t count = 0;

    // 从驱动获取已转换好的坐标
    bool pressed = esp_lcd_touch_get_coordinates(s_touch_handle, touch_x, touch_y, strength, &count, 1);

    if (pressed && count > 0)
    {
        if (x) *x = touch_x[0];
        if (y) *y = touch_y[0];
        if (points_num) *points_num = count;
    }
    else
    {
        if (x) *x = 0;
        if (y) *y = 0;
        if (points_num) *points_num = 0;
    }

    return ESP_OK;
}

esp_lcd_touch_handle_t bsp_touch_get_handle(void)
{
    return s_touch_handle;
}

esp_err_t bsp_touch_deinit(void)
{
    if (s_touch_handle == NULL)
    {
        ESP_LOGW(TAG, "Touch driver not initialized");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Deinitializing touch driver...");

    // esp_lcd_touch 组件没有提供专门的 delete 函数
    // 需要通过 esp_lcd_panel_io_del 删除 IO 句柄
    // 这里我们只能将句柄置空，因为触摸 IO 句柄由驱动内部管理
    s_touch_handle = NULL;

    ESP_LOGI(TAG, "Touch driver deinitialized successfully");
    return ESP_OK;
}
