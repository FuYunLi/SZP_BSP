/**
 * @file es7210.c
 * @brief ES7210 音频 ADC 极简本地驱动源文件 (基于 driver_ng I2C)
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "es7210.h"
#include "esp_log.h"
#include "esp_check.h"

static const char *TAG = "ES7210";

/* ES7210 内部寄存器地址定义 */
#define ES7210_RESET_REG00          0x00
#define ES7210_CLOCK_OFF_REG01      0x01
#define ES7210_MAINCLK_REG02        0x02
#define ES7210_MASTER_CLK_REG03     0x03
#define ES7210_LRCK_DIVH_REG04      0x04
#define ES7210_LRCK_DIVL_REG05      0x05
#define ES7210_POWER_DOWN_REG06     0x06
#define ES7210_OSR_REG07            0x07
#define ES7210_MODE_CONFIG_REG08    0x08
#define ES7210_TIME_CONTROL0_REG09  0x09
#define ES7210_TIME_CONTROL1_REG0A  0x0A
#define ES7210_SDP_INTERFACE1_REG11 0x11
#define ES7210_SDP_INTERFACE2_REG12 0x12
#define ES7210_ADC34_HPF2_REG20     0x20
#define ES7210_ADC34_HPF1_REG21     0x21
#define ES7210_ADC12_HPF1_REG22     0x22
#define ES7210_ADC12_HPF2_REG23     0x23
#define ES7210_ANALOG_REG40         0x40
#define ES7210_MIC12_BIAS_REG41     0x41
#define ES7210_MIC34_BIAS_REG42     0x42
#define ES7210_MIC1_GAIN_REG43      0x43
#define ES7210_MIC2_GAIN_REG44      0x44
#define ES7210_MIC3_GAIN_REG45      0x45
#define ES7210_MIC4_GAIN_REG46      0x46
#define ES7210_MIC1_POWER_REG47     0x47
#define ES7210_MIC2_POWER_REG48     0x48
#define ES7210_MIC3_POWER_REG49     0x49
#define ES7210_MIC4_POWER_REG4A     0x4A
#define ES7210_MIC12_POWER_REG4B    0x4B
#define ES7210_MIC34_POWER_REG4C    0x4C

typedef struct {
    i2c_master_dev_handle_t i2c_dev;
    uint16_t dev_addr;
    uint8_t mic_selected;
    float gain_db;
    bool master_mode;
} es7210_dev_t;

/* 增益映射结构体 */
typedef struct {
    float db;
    uint8_t reg_val;
} gain_map_t;

static const gain_map_t s_gain_map[] = {
    {0.0,   0x00}, {3.0,   0x01}, {6.0,   0x02}, {9.0,   0x03},
    {12.0,  0x04}, {15.0,  0x05}, {18.0,  0x06}, {21.0,  0x07},
    {24.0,  0x08}, {27.0,  0x09}, {30.0,  0x0A}, {33.0,  0x0B},
    {34.5,  0x0C}, {36.0,  0x0D}, {37.5,  0x0E}
};

static inline esp_err_t es7210_write_reg(es7210_handle_t dev, uint8_t reg_addr, uint8_t data)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(es->i2c_dev, write_buf, sizeof(write_buf), 1000);
}

static inline esp_err_t es7210_read_reg(es7210_handle_t dev, uint8_t reg_addr, uint8_t *reg_value)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    return i2c_master_transmit_receive(es->i2c_dev, &reg_addr, 1, reg_value, 1, 1000);
}

static esp_err_t es7210_update_reg_bit(es7210_handle_t dev, uint8_t reg_addr, uint8_t mask, uint8_t value)
{
    uint8_t reg_val = 0;
    ESP_RETURN_ON_ERROR(es7210_read_reg(dev, reg_addr, &reg_val), TAG, "读寄存器 0x%02X 失败", reg_addr);
    reg_val = (reg_val & (~mask)) | (value & mask);
    return es7210_write_reg(dev, reg_addr, reg_val);
}

static uint8_t get_gain_reg_value(float db)
{
    // 寻找最接近的增益设置
    uint8_t best_val = 0x00;
    float min_diff = 100.0;
    for (int i = 0; i < sizeof(s_gain_map) / sizeof(s_gain_map[0]); i++) {
        float diff = fabsf(s_gain_map[i].db - db);
        if (diff < min_diff) {
            min_diff = diff;
            best_val = s_gain_map[i].reg_val;
        }
    }
    return best_val;
}

es7210_handle_t es7210_create(i2c_master_bus_handle_t bus_handle, uint16_t dev_addr)
{
    if (bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C 总线句柄无效");
        return NULL;
    }

    es7210_dev_t *es = (es7210_dev_t *)calloc(1, sizeof(es7210_dev_t));
    if (es == NULL) {
        ESP_LOGE(TAG, "申请内存空间失败");
        return NULL;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = 100000, // 100kHz 标准速率，提供最高的兼容性与稳定性
    };

    if (i2c_master_bus_add_device(bus_handle, &dev_cfg, &es->i2c_dev) != ESP_OK) {
        ESP_LOGE(TAG, "添加 I2C 外设失败 (地址: 0x%02X)", dev_addr);
        free(es);
        return NULL;
    }

    es->dev_addr = dev_addr;
    ESP_LOGI(TAG, "添加 I2C 实例成功 (地址: 0x%02X)", dev_addr);
    return (es7210_handle_t)es;
}

esp_err_t es7210_init(es7210_handle_t dev, const es7210_config_t *cfg)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    ESP_RETURN_ON_FALSE(es != NULL, ESP_ERR_INVALID_ARG, TAG, "dev 句柄无效");
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg 句柄无效");

    es->mic_selected = cfg->mic_selected;
    es->gain_db = cfg->gain_db;
    es->master_mode = cfg->master_mode;

    ESP_LOGI(TAG, "配置 ES7210 寄存器初始化...");

    // 1. 软件复位与下电，进入安全状态
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_RESET_REG00, 0xFF), TAG, "写 REG00 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_RESET_REG00, 0x41), TAG, "写 REG00 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_CLOCK_OFF_REG01, 0x3F), TAG, "写 REG01 失败");
    
    // 2. 配置芯片基本状态周期及 HPF 滤波器参数
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_TIME_CONTROL0_REG09, 0x30), TAG, "写 REG09 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_TIME_CONTROL1_REG0A, 0x30), TAG, "写 REG0A 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_ADC12_HPF2_REG23, 0x2A), TAG, "写 REG23 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_ADC12_HPF1_REG22, 0x0A), TAG, "写 REG22 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_ADC34_HPF2_REG20, 0x0A), TAG, "写 REG20 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_ADC34_HPF1_REG21, 0x2A), TAG, "写 REG21 失败");

    // 3. 配置主从模式 (仅实现从机模式，因开发板为主机提供 MCLK 和时钟)
    if (es->master_mode) {
        ESP_LOGE(TAG, "当前驱动仅优化了 Slave 从机模式适配！");
        return ESP_ERR_NOT_SUPPORTED;
    } else {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MODE_CONFIG_REG08, 0x01, 0x00), TAG, "写 REG08 从机模式失败");
    }

    // 4. 配置模拟偏置参数：VDDA = 3.3V，MIC1/2 和 MIC3/4 偏置电压设为 2.87V
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_ANALOG_REG40, 0x43), TAG, "写 REG40 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC12_BIAS_REG41, 0x70), TAG, "写 REG41 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC34_BIAS_REG42, 0x70), TAG, "写 REG42 失败");

    // 5. 过采样率及分频清除设置
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_OSR_REG07, 0x20), TAG, "写 REG07 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MAINCLK_REG02, 0xC1), TAG, "写 REG02 失败");

    // 6. 配置 SDP 接口参数为：16-bit 格式，I2S 标准通讯协议
    // 写入 0x60 表示 16-bit
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_SDP_INTERFACE1_REG11, 0x60), TAG, "写 REG11 失败");

    // 7. 打开对应的麦克风偏置及前级电源，并配置初始增益
    ESP_RETURN_ON_ERROR(es7210_mic_select(dev, es->mic_selected), TAG, "麦克风通道选择失败");
    ESP_RETURN_ON_ERROR(es7210_set_gain(dev, es->gain_db), TAG, "设置初始模拟增益失败");

    // 8. 退出低功耗，完全使能内部时钟及 ADC 软上电
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_POWER_DOWN_REG06, 0x00), TAG, "写 REG06 上电失败");
    
    // 触发软件复位使设置生效并启动
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_RESET_REG00, 0x71), TAG, "写 REG00 触发复位失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_RESET_REG00, 0x41), TAG, "写 REG00 退出复位失败");

    ESP_LOGI(TAG, "ES7210 ADC 硬件配置并初始化完毕成功！");
    return ESP_OK;
}

esp_err_t es7210_mic_select(es7210_handle_t dev, uint8_t mic_mask)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    ESP_RETURN_ON_FALSE(es != NULL, ESP_ERR_INVALID_ARG, TAG, "dev 句柄无效");

    if (mic_mask == 0) {
        ESP_LOGE(TAG, "至少需要选择一个麦克风通道");
        return ESP_ERR_INVALID_ARG;
    }

    es->mic_selected = mic_mask;

    // 先关闭四个通道的前级模拟选通，并断电模拟偏置
    for (int i = 0; i < 4; i++) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC1_GAIN_REG43 + i, 0x10, 0x00), TAG, "清除通道增益控制失败");
    }
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC12_POWER_REG4B, 0xFF), TAG, "写 REG4B 失败");
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC34_POWER_REG4C, 0xFF), TAG, "写 REG4C 失败");

    uint8_t clk_off_mask = 0x00;

    // 根据选定的通道，拉高前级模拟使能 (0x10) 并上电对应的通道偏置
    uint8_t gain_reg_val = get_gain_reg_value(es->gain_db);

    if (mic_mask & ES7210_INPUT_MIC1) {
        ESP_LOGI(TAG, "启用麦克风通道: MIC1");
        clk_off_mask |= 0x0B; // 清零 REG01 中的 bit0, bit1, bit3
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC12_POWER_REG4B, 0x00), TAG, "开启 MIC1/2 模拟电源失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC1_GAIN_REG43, 0x1F, 0x10 | gain_reg_val), TAG, "使能 MIC1 选通与增益失败");
    }
    if (mic_mask & ES7210_INPUT_MIC2) {
        ESP_LOGI(TAG, "启用麦克风通道: MIC2");
        clk_off_mask |= 0x0B;
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC12_POWER_REG4B, 0x00), TAG, "开启 MIC1/2 模拟电源失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC2_GAIN_REG44, 0x1F, 0x10 | gain_reg_val), TAG, "使能 MIC2 选通与增益失败");
    }
    if (mic_mask & ES7210_INPUT_MIC3) {
        ESP_LOGI(TAG, "启用麦克风通道: MIC3");
        clk_off_mask |= 0x15; // 清零 REG01 中的 bit0, bit2, bit4
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC34_POWER_REG4C, 0x00), TAG, "开启 MIC3/4 模拟电源失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC3_GAIN_REG45, 0x1F, 0x10 | gain_reg_val), TAG, "使能 MIC3 选通与增益失败");
    }
    if (mic_mask & ES7210_INPUT_MIC4) {
        ESP_LOGI(TAG, "启用麦克风通道: MIC4");
        clk_off_mask |= 0x15;
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_MIC34_POWER_REG4C, 0x00), TAG, "开启 MIC3/4 模拟电源失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC4_GAIN_REG46, 0x1F, 0x10 | gain_reg_val), TAG, "使能 MIC4 选通与增益失败");
    }

    // 根据选定的通道清除对应的时钟关闭位
    uint8_t clk_off_val = 0x3F & (~clk_off_mask);
    ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_CLOCK_OFF_REG01, clk_off_val), TAG, "清除时钟关闭寄存器失败");

    // 根据麦克风通道数决定是立体声正常模式还是多通道 TDM 模式
    // 立创实战派默认只启用 MIC1 & MIC2 做双通道立体声，则此处将 SDP 接口 2 (0x12) 设为 0x00 (非 TDM)
    uint8_t active_mics = 0;
    for (int i = 0; i < 4; i++) {
        if (mic_mask & (1 << i)) active_mics++;
    }

    if (active_mics > 2) {
        ESP_LOGI(TAG, "启用多通道 TDM 录音模式 (active_mics = %d)", active_mics);
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_SDP_INTERFACE2_REG12, 0x02), TAG, "写 REG12 失败");
    } else {
        ESP_LOGI(TAG, "启用标准双声道立体声 I2S 录音模式");
        ESP_RETURN_ON_ERROR(es7210_write_reg(dev, ES7210_SDP_INTERFACE2_REG12, 0x00), TAG, "写 REG12 失败");
    }

    return ESP_OK;
}

esp_err_t es7210_set_gain(es7210_handle_t dev, float gain_db)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    ESP_RETURN_ON_FALSE(es != NULL, ESP_ERR_INVALID_ARG, TAG, "dev 句柄无效");

    if (gain_db < 0.0 || gain_db > 37.5) {
        ESP_LOGE(TAG, "增益数值超出许可范围 (0dB ~ 37.5dB): %.1f", gain_db);
        return ESP_ERR_INVALID_ARG;
    }

    es->gain_db = gain_db;
    uint8_t gain_reg_val = get_gain_reg_value(gain_db);

    // 仅针对已经选通的麦克风调整增益
    if (es->mic_selected & ES7210_INPUT_MIC1) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC1_GAIN_REG43, 0x0F, gain_reg_val), TAG, "写 REG43 失败");
    }
    if (es->mic_selected & ES7210_INPUT_MIC2) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC2_GAIN_REG44, 0x0F, gain_reg_val), TAG, "写 REG44 失败");
    }
    if (es->mic_selected & ES7210_INPUT_MIC3) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC3_GAIN_REG45, 0x0F, gain_reg_val), TAG, "写 REG45 失败");
    }
    if (es->mic_selected & ES7210_INPUT_MIC4) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, ES7210_MIC4_GAIN_REG46, 0x0F, gain_reg_val), TAG, "写 REG46 失败");
    }

    ESP_LOGI(TAG, "配置模拟增益为: %.1f dB (对应寄存器值: 0x%02X)", gain_db, gain_reg_val);
    return ESP_OK;
}

esp_err_t es7210_set_mute(es7210_handle_t dev, bool mute)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    ESP_RETURN_ON_FALSE(es != NULL, ESP_ERR_INVALID_ARG, TAG, "dev 句柄无效");

    // 根据官方库，mute 控制寄存器 0x14/0x15 控制数字部分静音
    if (mute) {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, 0x14, 0x03, 0x03), TAG, "静音 REG14 失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, 0x15, 0x03, 0x03), TAG, "静音 REG15 失败");
    } else {
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, 0x14, 0x03, 0x00), TAG, "取消静音 REG14 失败");
        ESP_RETURN_ON_ERROR(es7210_update_reg_bit(dev, 0x15, 0x03, 0x00), TAG, "取消静音 REG15 失败");
    }

    ESP_LOGI(TAG, "麦克风已设置为: %s", mute ? "静音" : "使能");
    return ESP_OK;
}

esp_err_t es7210_delete(es7210_handle_t dev)
{
    es7210_dev_t *es = (es7210_dev_t *)dev;
    if (es == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // 关闭麦克风偏置及芯片模拟电源以达到节能目的
    es7210_write_reg(dev, ES7210_MIC1_POWER_REG47, 0xFF);
    es7210_write_reg(dev, ES7210_MIC2_POWER_REG48, 0xFF);
    es7210_write_reg(dev, ES7210_MIC3_POWER_REG49, 0xFF);
    es7210_write_reg(dev, ES7210_MIC4_POWER_REG4A, 0xFF);
    es7210_write_reg(dev, ES7210_MIC12_POWER_REG4B, 0xFF);
    es7210_write_reg(dev, ES7210_MIC34_POWER_REG4C, 0xFF);
    es7210_write_reg(dev, ES7210_ANALOG_REG40, 0xC0);
    es7210_write_reg(dev, ES7210_CLOCK_OFF_REG01, 0x7F);
    es7210_write_reg(dev, ES7210_POWER_DOWN_REG06, 0x07);

    // 释放资源
    free(es);
    ESP_LOGI(TAG, "注销并注销 ES7210 芯片设备完成");
    return ESP_OK;
}
