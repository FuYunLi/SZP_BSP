#include <string.h>
#include <stdlib.h>
#include "driver/i2c_master.h"
#include "es8311.h"
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ES8311";

/* ES8311 内部寄存器地址定义 */
#define ES8311_RESET_REG00          0x00
#define ES8311_CLK_MANAGER_REG01    0x01
#define ES8311_CLK_MANAGER_REG02    0x02
#define ES8311_CLK_MANAGER_REG03    0x03
#define ES8311_CLK_MANAGER_REG04    0x04
#define ES8311_CLK_MANAGER_REG05    0x05
#define ES8311_CLK_MANAGER_REG06    0x06
#define ES8311_CLK_MANAGER_REG07    0x07
#define ES8311_CLK_MANAGER_REG08    0x08
#define ES8311_SDPIN_REG09          0x09
#define ES8311_SDPOUT_REG0A         0x0A
#define ES8311_SYSTEM_REG0D         0x0D
#define ES8311_SYSTEM_REG0E         0x0E
#define ES8311_SYSTEM_REG12         0x12
#define ES8311_SYSTEM_REG13         0x13
#define ES8311_SYSTEM_REG14         0x14
#define ES8311_ADC_REG15            0x15
#define ES8311_ADC_REG16            0x16
#define ES8311_ADC_REG17            0x17
#define ES8311_ADC_REG1C            0x1C
#define ES8311_DAC_REG31            0x31
#define ES8311_DAC_REG32            0x32
#define ES8311_DAC_REG37            0x37

typedef struct {
    i2c_master_dev_handle_t i2c_dev;
    uint16_t dev_addr;
} es8311_dev_t;

struct _coeff_div {
    uint32_t mclk;        /* mclk 频率 */
    uint32_t rate;        /* 采样率 */
    uint8_t pre_div;      /* 分频系数 1~8 */
    uint8_t pre_multi;    /* 预倍频选择 0:1x, 1:2x, 2:4x, 3:8x */
    uint8_t adc_div;      /* adcclk 分频 */
    uint8_t dac_div;      /* dacclk 分频 */
    uint8_t fs_mode;      /* fs 模式 0:ss, 1:ds */
    uint8_t lrck_h;       /* lrck 分频高位 */
    uint8_t lrck_l;       /* lrck 分频低位 */
    uint8_t bclk_div;     /* sclk 分频 */
    uint8_t adc_osr;      /* adc osr */
    uint8_t dac_osr;      /* dac osr */
};

/* 编解码器 MCLK 时钟分频系数表（移植自官方驱动） */
static const struct _coeff_div coeff_div[] = {
    /* 16k */
    {12288000, 16000, 0x03, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {18432000, 16000, 0x03, 0x01, 0x03, 0x03, 0x00, 0x02, 0xff, 0x0c, 0x10, 0x10},
    {16384000, 16000, 0x04, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {8192000,  16000, 0x02, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {6144000,  16000, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {4096000,  16000, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {3072000,  16000, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {2048000,  16000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {1536000,  16000, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {1024000,  16000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
};

static inline esp_err_t es8311_write_reg(es8311_handle_t dev, uint8_t reg_addr, uint8_t data)
{
    es8311_dev_t *es = (es8311_dev_t *)dev;
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(es->i2c_dev, write_buf, sizeof(write_buf), 1000);
}

static inline esp_err_t es8311_read_reg(es8311_handle_t dev, uint8_t reg_addr, uint8_t *reg_value)
{
    es8311_dev_t *es = (es8311_dev_t *)dev;
    return i2c_master_transmit_receive(es->i2c_dev, &reg_addr, 1, reg_value, 1, 1000);
}

static int get_coeff(uint32_t mclk, uint32_t rate)
{
    for (int i = 0; i < (sizeof(coeff_div) / sizeof(coeff_div[0])); i++) {
        if (coeff_div[i].rate == rate && coeff_div[i].mclk == mclk) {
            return i;
        }
    }
    return -1;
}

esp_err_t es8311_sample_frequency_config(es8311_handle_t dev, int mclk_frequency, int sample_frequency)
{
    uint8_t regv;
    int coeff = get_coeff(mclk_frequency, sample_frequency);
    if (coeff < 0) {
        ESP_LOGE(TAG, "无法配置采样率 %dHz，MCLK 频点为 %dHz", sample_frequency, mclk_frequency);
        return ESP_ERR_INVALID_ARG;
    }

    const struct _coeff_div *const selected_coeff = &coeff_div[coeff];

    // REG02
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_CLK_MANAGER_REG02, &regv), TAG, "I2C 读写失败");
    regv &= 0x07;
    regv |= (selected_coeff->pre_div - 1) << 5;
    regv |= selected_coeff->pre_multi << 3;
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG02, regv), TAG, "I2C 读写失败");

    // REG03
    const uint8_t reg03 = (selected_coeff->fs_mode << 6) | selected_coeff->adc_osr;
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG03, reg03), TAG, "I2C 读写失败");

    // REG04
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG04, selected_coeff->dac_osr), TAG, "I2C 读写失败");

    // REG05
    const uint8_t reg05 = ((selected_coeff->adc_div - 1) << 4) | (selected_coeff->dac_div - 1);
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG05, reg05), TAG, "I2C 读写失败");

    // REG06
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_CLK_MANAGER_REG06, &regv), TAG, "I2C 读写失败");
    regv &= 0xE0;
    if (selected_coeff->bclk_div < 19) {
        regv |= (selected_coeff->bclk_div - 1) << 0;
    } else {
        regv |= (selected_coeff->bclk_div) << 0;
    }
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG06, regv), TAG, "I2C 读写失败");

    // REG07
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_CLK_MANAGER_REG07, &regv), TAG, "I2C 读写失败");
    regv &= 0xC0;
    regv |= selected_coeff->lrck_h << 0;
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG07, regv), TAG, "I2C 读写失败");

    // REG08
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG08, selected_coeff->lrck_l), TAG, "I2C 读写失败");

    return ESP_OK;
}

static esp_err_t es8311_clock_config(es8311_handle_t dev, const es8311_clock_config_t *const clk_cfg, es8311_resolution_t res)
{
    uint8_t reg06;
    uint8_t reg01 = 0x3F; // 使能所有内部时钟
    int mclk_hz;

    if (clk_cfg->mclk_from_mclk_pin) {
        mclk_hz = clk_cfg->mclk_frequency;
    } else {
        mclk_hz = clk_cfg->sample_frequency * (int)res * 2;
        reg01 |= (1 << 7); // 选择 BCLK (SCK) 脚
    }

    if (clk_cfg->mclk_inverted) {
        reg01 |= (1 << 6); // 反相 MCLK
    }
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, reg01), TAG, "I2C 读写失败");

    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_CLK_MANAGER_REG06, &reg06), TAG, "I2C 读写失败");
    if (clk_cfg->sclk_inverted) {
        reg06 |= (1 << 5);
    } else {
        reg06 &= ~(1 << 5);
    }
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_CLK_MANAGER_REG06, reg06), TAG, "I2C 读写失败");

    return es8311_sample_frequency_config(dev, mclk_hz, clk_cfg->sample_frequency);
}

static esp_err_t es8311_resolution_config(const es8311_resolution_t res, uint8_t *reg)
{
    switch (res) {
    case ES8311_RESOLUTION_16:
        *reg |= (3 << 2);
        break;
    case ES8311_RESOLUTION_18:
        *reg |= (2 << 2);
        break;
    case ES8311_RESOLUTION_20:
        *reg |= (1 << 2);
        break;
    case ES8311_RESOLUTION_24:
        *reg |= (0 << 2);
        break;
    case ES8311_RESOLUTION_32:
        *reg |= (4 << 2);
        break;
    default:
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

static esp_err_t es8311_fmt_config(es8311_handle_t dev, const es8311_resolution_t res_in, const es8311_resolution_t res_out)
{
    uint8_t reg09 = 0;
    uint8_t reg0a = 0;

    ESP_LOGI(TAG, "配置 ES8311 处于 Slave 从模式和标准 I2S 格式");
    uint8_t reg00;
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_RESET_REG00, &reg00), TAG, "I2C 读写失败");
    reg00 &= 0xBF; // 将 bit6 置 0 为从模式
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_RESET_REG00, reg00), TAG, "I2C 读写失败");

    es8311_resolution_config(res_in, &reg09);
    es8311_resolution_config(res_out, &reg0a);

    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SDPIN_REG09, reg09), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SDPOUT_REG0A, reg0a), TAG, "I2C 读写失败");

    return ESP_OK;
}

es8311_handle_t es8311_create(i2c_master_bus_handle_t bus_handle, const uint16_t dev_addr)
{
    if (bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C 总线句柄无效");
        return NULL;
    }

    es8311_dev_t *dev = (es8311_dev_t *)calloc(1, sizeof(es8311_dev_t));
    if (dev == NULL) {
        ESP_LOGE(TAG, "申请内存失败");
        return NULL;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = 100000,
    };

    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_config, &dev->i2c_dev);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "添加 I2C 设备失败: %s", esp_err_to_name(err));
        free(dev);
        return NULL;
    }

    dev->dev_addr = dev_addr;
    return (es8311_handle_t)dev;
}

void es8311_delete(es8311_handle_t dev)
{
    if (dev) {
        es8311_dev_t *es = (es8311_dev_t *)dev;
        i2c_master_bus_rm_device(es->i2c_dev);
        free(es);
    }
}

esp_err_t es8311_init(es8311_handle_t dev, const es8311_clock_config_t *const clk_cfg, 
                      const es8311_resolution_t res_in, const es8311_resolution_t res_out)
{
    ESP_RETURN_ON_FALSE(
        (clk_cfg->sample_frequency >= 8000) && (clk_cfg->sample_frequency <= 96000),
        ESP_ERR_INVALID_ARG, TAG, "采样率需要在 [8000; 96000] Hz 范围内"
    );

    /* 重启 ES8311 芯片并上电 */
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_RESET_REG00, 0x1F), TAG, "I2C 读写失败");
    vTaskDelay(pdMS_TO_TICKS(20));
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_RESET_REG00, 0x00), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_RESET_REG00, 0x80), TAG, "I2C 读写失败"); // 上电指令

    /* 配置时钟 */
    ESP_RETURN_ON_ERROR(es8311_clock_config(dev, clk_cfg, res_out), TAG, "时钟配置失败");

    /* 配置数据分辨率和从模式 */
    ESP_RETURN_ON_ERROR(es8311_fmt_config(dev, res_in, res_out), TAG, "格式配置失败");

    // 默认配置寄存器写入上电逻辑
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SYSTEM_REG0D, 0x01), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SYSTEM_REG0E, 0x02), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SYSTEM_REG12, 0x00), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_SYSTEM_REG13, 0x10), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_ADC_REG1C, 0x6A), TAG, "I2C 读写失败");
    ESP_RETURN_ON_ERROR(es8311_write_reg(dev, ES8311_DAC_REG37, 0x08), TAG, "I2C 读写失败");

    return ESP_OK;
}

esp_err_t es8311_voice_volume_set(es8311_handle_t dev, int volume, int *volume_set)
{
    if (volume < 0) {
        volume = 0;
    } else if (volume > 100) {
        volume = 100;
    }

    int reg32;
    if (volume == 0) {
        reg32 = 0;
    } else {
        reg32 = ((volume) * 256 / 100) - 1;
    }

    if (volume_set != NULL) {
        *volume_set = volume;
    }
    return es8311_write_reg(dev, ES8311_DAC_REG32, reg32);
}

esp_err_t es8311_voice_volume_get(es8311_handle_t dev, int *volume)
{
    uint8_t reg32;
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_DAC_REG32, &reg32), TAG, "I2C 读写失败");

    if (reg32 == 0) {
        *volume = 0;
    } else {
        *volume = ((reg32 * 100) / 256) + 1;
    }
    return ESP_OK;
}

esp_err_t es8311_voice_mute(es8311_handle_t dev, bool mute)
{
    uint8_t reg31;
    ESP_RETURN_ON_ERROR(es8311_read_reg(dev, ES8311_DAC_REG31, &reg31), TAG, "I2C 读写失败");

    if (mute) {
        reg31 |= (1 << 6) | (1 << 5);
    } else {
        reg31 &= ~((1 << 6) | (1 << 5));
    }

    return es8311_write_reg(dev, ES8311_DAC_REG31, reg31);
}
