/**
 * @file app_cli.c
 * @brief 应用层命令行接口实现，用于集中注册与管理当前项目的诊断、控制及测试命令
 */

#include "app_cli.h"
#include "console_service.h"
#include "bsp_backlight.h"
#include "esp_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "bsp_key.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "bsp_i2c.h"
#include "pca9557.h"
#include "bsp_littlefs.h"
#include "bsp_sdcard.h"
#include <dirent.h>

static const char *TAG = "app_cli";

/* 版本查询命令的回调函数 */
static int do_version_cmd(int argc, char **argv)
{
    printf("立创实战派 S3 BSP v1.0.0\n");
    return 0;
}

/* 屏幕背光控制命令的回调函数 */
static int do_backlight_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: backlight <0|1>\n");
        return 1;
    }
    
    int enable = atoi(argv[1]);
    if (enable != 0 && enable != 1)
    {
        printf("Invalid argument. Usage: backlight <0|1>\n");
        return 1;
    }
    
    esp_err_t err = bsp_backlight_set(enable == 1);
    if (err != ESP_OK)
    {
        printf("Failed to set backlight state: %s\n", esp_err_to_name(err));
        return 1;
    }
    
    printf("Backlight set to %s\n", enable ? "ON" : "OFF");
    return 0;
}

/* 按键状态查询命令的回调函数 */
static int do_key_cmd(int argc, char **argv)
{
    bool pressed = bsp_key_is_pressed();
    printf("BOOT Key status: %s (Level: %d)\n", pressed ? "PRESSED" : "RELEASED", gpio_get_level(0));
    return 0;
}

/* NVS 读写测试命令的回调函数 */
static int do_nvs_test_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: nvs_test <key> [value]\n");
        return 1;
    }

    const char *key = argv[1];
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return 1;
    }

    if (argc >= 3)
    {
        // 写入模式
        const char *val_str = argv[2];
        char *endptr;
        long val_int = strtol(val_str, &endptr, 10);
        
        if (*endptr == '\0')
        {
            // 纯数字，写入整型 i32
            err = nvs_set_i32(my_handle, key, (int32_t)val_int);
            if (err == ESP_OK)
            {
                err = nvs_commit(my_handle);
            }
            if (err == ESP_OK)
            {
                printf("NVS Write Success: Integer %s = %ld\n", key, val_int);
            }
            else
            {
                printf("Failed to write integer: %s\n", esp_err_to_name(err));
            }
        }
        else
        {
            // 字符串，写入 str
            err = nvs_set_str(my_handle, key, val_str);
            if (err == ESP_OK)
            {
                err = nvs_commit(my_handle);
            }
            if (err == ESP_OK)
            {
                printf("NVS Write Success: String %s = \"%s\"\n", key, val_str);
            }
            else
            {
                printf("Failed to write string: %s\n", esp_err_to_name(err));
            }
        }
    }
    else
    {
        // 读取模式
        int32_t val_int = 0;
        err = nvs_get_i32(my_handle, key, &val_int);
        if (err == ESP_OK)
        {
            printf("Read integer: %s = %ld\n", key, (long)val_int);
        }
        else if (err == ESP_ERR_NVS_TYPE_MISMATCH || err == ESP_ERR_NVS_NOT_FOUND)
        {
            size_t required_size = 0;
            err = nvs_get_str(my_handle, key, NULL, &required_size);
            if (err == ESP_OK)
            {
                char *buf = malloc(required_size);
                if (buf != NULL)
                {
                    err = nvs_get_str(my_handle, key, buf, &required_size);
                    if (err == ESP_OK)
                    {
                        printf("Read string: %s = \"%s\"\n", key, buf);
                    }
                    else
                    {
                        printf("Failed to read string: %s\n", esp_err_to_name(err));
                    }
                    free(buf);
                }
                else
                {
                    printf("Memory allocation failed\n");
                }
            }
            else if (err == ESP_ERR_NVS_NOT_FOUND)
            {
                printf("Key '%s' not found in NVS namespace 'storage'\n", key);
            }
            else
            {
                printf("Failed to read string size: %s\n", esp_err_to_name(err));
            }
        }
        else
        {
            printf("Failed to read key '%s': %s\n", key, esp_err_to_name(err));
        }
    }

    nvs_close(my_handle);
    return 0;
}

/* I2C 总线扫描检测命令的回调函数 */
static int do_i2c_scan_cmd(int argc, char **argv)
{
    i2c_master_bus_handle_t bus_handle = bsp_i2c_get_bus_handle();
    if (bus_handle == NULL)
    {
        printf("Error: I2C1 bus not initialized.\n");
        return 1;
    }

    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16)
    {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++)
        {
            uint8_t address = i + j;
            // 0x00 - 0x02, 0x78 - 0x7f are reserved addresses in I2C specification
            if (address < 0x03 || address > 0x77)
            {
                printf("   ");
                continue;
            }
            
            esp_err_t ret = i2c_master_probe(bus_handle, address, 50);
            if (ret == ESP_OK)
            {
                printf("%02x ", address);
            }
            else
            {
                printf("-- ");
            }
        }
        printf("\r\n");
    }

    return 0;
}

/* PCA9557 设置引脚输出电平回调函数 */
static int do_pca9557_set_cmd(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: pca9557_set <pin> <0|1>\n");
        return 1;
    }

    int pin = atoi(argv[1]);
    int level = atoi(argv[2]);

    if (pin < 0 || pin > 7)
    {
        printf("Error: Pin must be 0-7.\n");
        return 1;
    }

    if (level != 0 && level != 1)
    {
        printf("Error: Level must be 0 or 1.\n");
        return 1;
    }

    // 先设置为输出模式
    esp_err_t err = pca9557_set_config(pin, false);
    if (err != ESP_OK)
    {
        printf("Error: Failed to configure pin as output: %s\n", esp_err_to_name(err));
        return 1;
    }

    // 设置输出电平
    err = pca9557_set_output_level(pin, level);
    if (err != ESP_OK)
    {
        printf("Error: Failed to set pin output level: %s\n", esp_err_to_name(err));
        return 1;
    }

    printf("PCA9557 Pin %d output level set to %d.\n", pin, level);
    return 0;
}

/* PCA9557 获取引脚状态回调函数 */
static int do_pca9557_get_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: pca9557_get <pin>\n");
        return 1;
    }

    int pin = atoi(argv[1]);
    if (pin < 0 || pin > 7)
    {
        printf("Error: Pin must be 0-7.\n");
        return 1;
    }

    uint8_t input_level = 0;
    uint8_t output_level = 0;

    esp_err_t err = pca9557_get_input_level(pin, &input_level);
    if (err != ESP_OK)
    {
        printf("Error: Failed to read input level: %s\n", esp_err_to_name(err));
        return 1;
    }

    err = pca9557_get_output_level(pin, &output_level);
    if (err != ESP_OK)
    {
        printf("Error: Failed to read output level: %s\n", esp_err_to_name(err));
        return 1;
    }

    printf("PCA9557 Pin %d status:\n", pin);
    printf("  Input Level  : %d\n", input_level);
    printf("  Output Level : %d (defined if configured as output)\n", output_level);

    return 0;
}

/* LittleFS 写入测试命令的回调函数 */
static int do_fs_write_cmd(int argc, char **argv)
{
    if (!bsp_littlefs_is_mounted())
    {
        printf("Error: LittleFS partition is not mounted.\n");
        return 1;
    }

    if (argc < 3)
    {
        printf("Usage: fs_write <file> <data>\n");
        return 1;
    }

    const char *file_name = argv[1];
    const char *data = argv[2];
    char path[128];
    snprintf(path, sizeof(path), "/littlefs/%s", file_name);

    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        printf("Error: Failed to open file %s for writing\n", path);
        return 1;
    }

    size_t written = fwrite(data, 1, strlen(data), f);
    fclose(f);

    if (written < strlen(data))
    {
        printf("Error: Only wrote %zu of %zu bytes to %s\n", written, strlen(data), path);
        return 1;
    }

    printf("Successfully wrote %zu bytes to file %s\n", written, path);
    return 0;
}

/* LittleFS 读取测试命令的回调函数 */
static int do_fs_read_cmd(int argc, char **argv)
{
    if (!bsp_littlefs_is_mounted())
    {
        printf("Error: LittleFS partition is not mounted.\n");
        return 1;
    }

    if (argc < 2)
    {
        printf("Usage: fs_read <file>\n");
        return 1;
    }

    const char *file_name = argv[1];
    char path[128];
    snprintf(path, sizeof(path), "/littlefs/%s", file_name);

    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        printf("Error: Failed to open file %s for reading\n", path);
        return 1;
    }

    char buf[256];
    printf("Content of file %s:\n", path);
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        printf("%s", buf);
    }
    printf("\n");
    fclose(f);

    return 0;
}

/* MicroSD 卡目录文件列出测试命令的回调函数 */
static int do_sd_ls_cmd(int argc, char **argv)
{
    if (!bsp_sdcard_is_mounted())
    {
        printf("Error: SD card is not mounted.\n");
        return 1;
    }

    DIR *dir = opendir("/sdcard");
    if (dir == NULL)
    {
        printf("Error: Failed to open directory /sdcard\n");
        return 1;
    }

    printf("Files in /sdcard:\n");
    struct dirent *de;
    while ((de = readdir(dir)) != NULL)
    {
        printf("  %s\n", de->d_name);
    }
    closedir(dir);
    return 0;
}

/* MicroSD 卡文件写入测试命令的回调函数 */
static int do_sd_write_cmd(int argc, char **argv)
{
    if (!bsp_sdcard_is_mounted())
    {
        printf("Error: SD card is not mounted.\n");
        return 1;
    }

    if (argc < 3)
    {
        printf("Usage: sd_write <file> <data>\n");
        return 1;
    }

    const char *file_name = argv[1];
    const char *data = argv[2];
    char path[128];
    snprintf(path, sizeof(path), "/sdcard/%s", file_name);

    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        printf("Error: Failed to open file %s for writing\n", path);
        return 1;
    }

    size_t written = fwrite(data, 1, strlen(data), f);
    fclose(f);

    if (written < strlen(data))
    {
        printf("Error: Only wrote %zu of %zu bytes to %s\n", written, strlen(data), path);
        return 1;
    }

    printf("Successfully wrote %zu bytes to file %s\n", written, path);
    return 0;
}

/* MicroSD 卡文件读取测试命令的回调函数 */
static int do_sd_read_cmd(int argc, char **argv)
{
    if (!bsp_sdcard_is_mounted())
    {
        printf("Error: SD card is not mounted.\n");
        return 1;
    }

    if (argc < 2)
    {
        printf("Usage: sd_read <file>\n");
        return 1;
    }

    const char *file_name = argv[1];
    char path[128];
    snprintf(path, sizeof(path), "/sdcard/%s", file_name);

    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        printf("Error: Failed to open file %s for reading\n", path);
        return 1;
    }

    char buf[256];
    printf("Content of file %s:\n", path);
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        printf("%s", buf);
    }
    printf("\n");
    fclose(f);
    return 0;
}

/* 注册系统级的通用诊断指令 */
static void register_system_commands(void)
{
    const esp_console_cmd_t version_cmd = {
        .command = "version",
        .help = "Get system/BSP version information",
        .hint = NULL,
        .func = &do_version_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&version_cmd));

    const esp_console_cmd_t backlight_cmd = {
        .command = "backlight",
        .help = "Control LCD backlight: backlight <0|1>",
        .hint = NULL,
        .func = &do_backlight_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&backlight_cmd));

    const esp_console_cmd_t key_cmd = {
        .command = "key",
        .help = "Get BOOT key level and pressed status",
        .hint = NULL,
        .func = &do_key_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&key_cmd));

    const esp_console_cmd_t nvs_test_cmd = {
        .command = "nvs_test",
        .help = "Write or read NVS key-value pairs: nvs_test <key> [value]",
        .hint = NULL,
        .func = &do_nvs_test_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&nvs_test_cmd));

    const esp_console_cmd_t i2c_scan_cmd = {
        .command = "i2c_scan",
        .help = "Scan I2C1 bus for responding slave devices",
        .hint = NULL,
        .func = &do_i2c_scan_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&i2c_scan_cmd));

    const esp_console_cmd_t pca9557_set_cmd = {
        .command = "pca9557_set",
        .help = "Set PCA9557 pin output level: pca9557_set <pin> <0|1>",
        .hint = NULL,
        .func = &do_pca9557_set_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&pca9557_set_cmd));

    const esp_console_cmd_t pca9557_get_cmd = {
        .command = "pca9557_get",
        .help = "Get PCA9557 pin input and output levels: pca9557_get <pin>",
        .hint = NULL,
        .func = &do_pca9557_get_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&pca9557_get_cmd));

    const esp_console_cmd_t fs_write_cmd = {
        .command = "fs_write",
        .help = "Write string to LittleFS: fs_write <file> <data>",
        .hint = NULL,
        .func = &do_fs_write_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&fs_write_cmd));

    const esp_console_cmd_t fs_read_cmd = {
        .command = "fs_read",
        .help = "Read file from LittleFS: fs_read <file>",
        .hint = NULL,
        .func = &do_fs_read_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&fs_read_cmd));

    const esp_console_cmd_t sd_ls_cmd = {
        .command = "sd_ls",
        .help = "List files in SD card directory",
        .hint = NULL,
        .func = &do_sd_ls_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&sd_ls_cmd));

    const esp_console_cmd_t sd_write_cmd = {
        .command = "sd_write",
        .help = "Write string to SD card: sd_write <file> <data>",
        .hint = NULL,
        .func = &do_sd_write_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&sd_write_cmd));

    const esp_console_cmd_t sd_read_cmd = {
        .command = "sd_read",
        .help = "Read file from SD card: sd_read <file>",
        .hint = NULL,
        .func = &do_sd_read_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&sd_read_cmd));
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化并注册应用层的控制台命令
 */
esp_err_t app_cli_init(void)
{
    ESP_LOGI(TAG, "Initializing app CLI...");
    
    ESP_ERROR_CHECK(console_service_start());
    
    // 注册自定义的命令
    register_system_commands();
    
    return ESP_OK;
}