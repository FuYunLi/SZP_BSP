/**
 * @file app_cli.c
 * @brief 应用层命令行接口实现，用于集中注册与管理当前项目的诊断、控制及测试命令
 */

#include "app_cli.h"
#include "app_ui.h"
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
#include "qmi8658.h"
#include "bsp_power.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "wifi_service.h"
#include "ble_service.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <dirent.h>
#include <math.h>
#include <sys/select.h>
#include <unistd.h>

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

/* 进入睡眠模式诊断命令的回调函数 */
static int do_goto_sleep_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: goto_sleep <light|deep>\n");
        return 1;
    }
    
    const char *mode_str = argv[1];
    if (strcmp(mode_str, "light") == 0)
    {
        printf("Preparing to enter Light-sleep...\n");
        // 关闭屏幕背光以节省能耗
        bsp_backlight_set(false);
        // 刷新串口输出缓存以防信息截断
        fflush(stdout);
        fsync(fileno(stdout));
        vTaskDelay(pdMS_TO_TICKS(100));
        
        esp_err_t err = bsp_power_enter_sleep(BSP_SLEEP_MODE_LIGHT);
        if (err != ESP_OK)
        {
            printf("Failed to enter Light-sleep: %s\n", esp_err_to_name(err));
            bsp_backlight_set(true);
            return 1;
        }
        
        // 唤醒后重新开启背光并提示
        bsp_backlight_set(true);
        printf("System has woken up from Light-sleep.\n");
        return 0;
    }
    else if (strcmp(mode_str, "deep") == 0)
    {
        printf("Preparing to enter Deep-sleep...\n");
        bsp_backlight_set(false);
        fflush(stdout);
        fsync(fileno(stdout));
        vTaskDelay(pdMS_TO_TICKS(100));
        
        esp_err_t err = bsp_power_enter_sleep(BSP_SLEEP_MODE_DEEP);
        if (err != ESP_OK)
        {
            printf("Failed to enter Deep-sleep: %s\n", esp_err_to_name(err));
            bsp_backlight_set(true);
            return 1;
        }
        return 0;
    }
    else
    {
        printf("Invalid sleep mode. Usage: goto_sleep <light|deep>\n");
        return 1;
    }
}

/* LCD 清屏诊断命令的回调函数 */
static int do_lcd_clear_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("用法: lcd_clear <color_name | hex_val>\n");
        printf("可选颜色名称: red, green, blue, black, white, yellow\n");
        printf("十六进制格式: 0xF800 (RGB565)\n");
        return 1;
    }

    uint16_t color = 0x0000;
    const char *color_arg = argv[1];

    if (strcmp(color_arg, "red") == 0)
    {
        color = 0xF800;
    }
    else if (strcmp(color_arg, "green") == 0)
    {
        color = 0x07E0;
    }
    else if (strcmp(color_arg, "blue") == 0)
    {
        color = 0x001F;
    }
    else if (strcmp(color_arg, "black") == 0)
    {
        color = 0x0000;
    }
    else if (strcmp(color_arg, "white") == 0)
    {
        color = 0xFFFF;
    }
    else if (strcmp(color_arg, "yellow") == 0)
    {
        color = 0xFFE0;
    }
    else
    {
        char *endptr;
        long val = strtol(color_arg, &endptr, 0);
        if (endptr == color_arg || *endptr != '\0')
        {
            printf("无效的颜色输入: %s\n", color_arg);
            return 1;
        }
        color = (uint16_t)val;
    }

    printf("正在清屏为颜色值: 0x%04X...\n", color);
    esp_err_t err = bsp_lcd_clear(color);
    if (err != ESP_OK)
    {
        printf("清屏失败: %s\n", esp_err_to_name(err));
        return 1;
    }

    printf("清屏成功。\n");
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

/* 6 轴姿态传感器实时读取测试命令的回调函数 */
static int do_imu_read_cmd(int argc, char **argv)
{
    printf("IMU Data (Press any key to exit):\n");

    // 清空现有输入缓冲区
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    while (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
    {
        getchar();
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
    }

    while (1)
    {
        float acc[3];
        float gyro[3];
        float temp = 0.0f;

        esp_err_t err = qmi8658_read_parsed(acc, gyro);
        if (err == ESP_OK)
        {
            qmi8658_read_temp(&temp);

            // 通过重力分量计算 Pitch 与 Roll 倾角
            float pitch = atan2f(acc[0], sqrtf(acc[1] * acc[1] + acc[2] * acc[2])) * 57.29578f;
            float roll = atan2f(acc[1], acc[2]) * 57.29578f;

            // 实现在行首动态覆盖打印
            printf("\rAcc: X=%6.3fg, Y=%6.3fg, Z=%6.3fg | Gyro: X=%6.1fdps, Y=%6.1fdps, Z=%6.1fdps | Pitch: %6.1f deg, Roll: %6.1f deg | Temp: %4.1f C",
                   acc[0], acc[1], acc[2], gyro[0], gyro[1], gyro[2], pitch, roll, temp);
            fflush(stdout);
        }
        else
        {
            printf("\rError: Failed to read IMU data (%s)                   ", esp_err_to_name(err));
            fflush(stdout);
        }

        // 以 100ms 为周期轮询输入，如有输入则安全退出
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
        {
            getchar(); // 吃掉该按键，避免打印多余的回车或控制台报错
            break;
        }
    }

    printf("\nExit IMU reader.\n");
    return 0;
}

/* 电容触摸屏实时数据读取测试命令的回调函数 */
static int do_touch_read_cmd(int argc, char **argv)
{
    printf("Touch Panel Read (Press any key to exit):\n");
    printf("Touch the screen to cycle display colors: RED -> GREEN -> BLUE\n");

    // 清空现有输入缓冲区 (带10ms延迟等待防止输入竞态)
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10000;
    while (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
    {
        getchar();
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
    }

    // 初始化测试颜色和状态
    static const uint16_t test_colors[] = {0xF800, 0x07E0, 0x001F}; // 红, 绿, 蓝
    static const char *color_names[] = {"RED", "GREEN", "BLUE"};
    int color_idx = 0;
    bool last_pressed = false;

    // 开始清屏为黑色
    bsp_lcd_clear(0x0000);

    while (1)
    {
        uint16_t x = 0;
        uint16_t y = 0;
        uint8_t points_num = 0;

        esp_err_t err = bsp_touch_read(&x, &y, &points_num);
        if (err == ESP_OK)
        {
            bool current_pressed = (points_num > 0);
            
            if (current_pressed)
            {
                printf("\rPressed: TRUE  | Points: %d | X: %3d | Y: %3d                                   ", points_num, x, y);
                
                // 触碰上升沿切换颜色
                if (!last_pressed)
                {
                    bsp_lcd_clear(test_colors[color_idx]);
                    printf("\n[Touch] Toggled LCD color to %s\n", color_names[color_idx]);
                    color_idx = (color_idx + 1) % 3;
                }
            }
            else
            {
                printf("\rPressed: FALSE | Points: 0 | X: --- | Y: ---                                   ");
            }
            fflush(stdout);
        }
        else
        {
            printf("\rError: Failed to read touch data (%s)                   ", esp_err_to_name(err));
            fflush(stdout);
        }

        // 以 50ms 为周期轮询输入，如果是合法字符（非回车）则安全退出
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 50000;
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
        {
            int c = getchar();
            if (c != '\n' && c != '\r' && c != -1)
            {
                break;
            }
        }
    }

    printf("\nExit Touch reader.\n");
    // 测试结束清屏为黑色
    bsp_lcd_clear(0x0000);
    return 0;
}


/* UI Demo 切换诊断命令的回调函数 */
static int do_ui_demo_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("用法: ui_demo <default | widgets | benchmark | music | stress>\n");
        return 1;
    }
    
    const char *demo_name = argv[1];
    printf("正在切换 UI 演示 Demo 到: %s...\n", demo_name);
    app_ui_show_demo(demo_name);
    printf("UI 切换指令已下发。\n");
    return 0;
}

/* Wi-Fi 连接命令的回调函数 */
static int do_wifi_connect_cmd(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("用法: wifi_connect <ssid> <password>\n");
        return 1;
    }
    const char *ssid = argv[1];
    const char *pwd = argv[2];
    printf("正在连接 Wi-Fi, SSID: %s...\n", ssid);
    esp_err_t err = wifi_service_connect(ssid, pwd);
    if (err == ESP_OK)
    {
        printf("连接请求已下发，请在后台观察 IP 获取情况 (可使用 wifi_status 查询)。\n");
    }
    else
    {
        printf("连接启动失败: %s\n", esp_err_to_name(err));
    }
    return 0;
}

/* Wi-Fi 开启 AP 命令的回调函数 */
static int do_wifi_ap_cmd(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("用法: wifi_ap <ssid> [password]\n");
        return 1;
    }
    const char *ssid = argv[1];
    const char *pwd = (argc >= 3) ? argv[2] : NULL;
    printf("正在开启 SoftAP 热点, SSID: %s...\n", ssid);
    esp_err_t err = wifi_service_ap_start(ssid, pwd);
    if (err == ESP_OK)
    {
        printf("SoftAP 开启成功。\n");
    }
    else
    {
        printf("SoftAP 开启失败: %s\n", esp_err_to_name(err));
    }
    return 0;
}

/* Wi-Fi 清除配置命令的回调函数 */
static int do_wifi_clear_cmd(int argc, char **argv)
{
    printf("正在清除 NVS 中的 Wi-Fi 配置...\n");
    esp_err_t err = wifi_service_clear_config();
    if (err == ESP_OK)
    {
        printf("Wi-Fi 配置已成功清除。\n");
    }
    else
    {
        printf("配置清除失败: %s\n", esp_err_to_name(err));
    }
    return 0;
}

/* Wi-Fi 状态查询命令的回调函数 */
static int do_wifi_status_cmd(int argc, char **argv)
{
    wifi_service_state_t state = wifi_service_get_state();
    const char *state_str = "UNKNOWN";
    switch (state)
    {
        case WIFI_SERVICE_STATE_DISCONNECTED: state_str = "DISCONNECTED"; break;
        case WIFI_SERVICE_STATE_CONNECTING: state_str = "CONNECTING"; break;
        case WIFI_SERVICE_STATE_CONNECTED: state_str = "CONNECTED (WAITING FOR IP)"; break;
        case WIFI_SERVICE_STATE_GOT_IP: state_str = "CONNECTED (GOT IP)"; break;
        case WIFI_SERVICE_STATE_FAILED: state_str = "FAILED"; break;
    }
    printf("Wi-Fi 连接状态: %s\n", state_str);

    if (state == WIFI_SERVICE_STATE_GOT_IP)
    {
        esp_netif_ip_info_t ip_info;
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
        {
            printf("IP 地址  : " IPSTR "\n", IP2STR(&ip_info.ip));
            printf("子网掩码 : " IPSTR "\n", IP2STR(&ip_info.netmask));
            printf("网关     : " IPSTR "\n", IP2STR(&ip_info.gw));
        }
    }
    return 0;
}

/* BLE 广播启动命令的回调函数 */
static int do_ble_start_cmd(int argc, char **argv)
{
    printf("正在启动 BLE 蓝牙广播...\n");
    esp_err_t err = ble_service_start();
    if (err == ESP_OK)
    {
        printf("蓝牙广播已成功启动。\n");
    }
    else
    {
        printf("蓝牙广播启动失败: %s\n", esp_err_to_name(err));
    }
    return 0;
}

/* BLE 广播停止命令的回调函数 */
static int do_ble_stop_cmd(int argc, char **argv)
{
    printf("正在停止 BLE 蓝牙广播...\n");
    esp_err_t err = ble_service_stop();
    if (err == ESP_OK)
    {
        printf("蓝牙广播已停止。\n");
    }
    else
    {
        printf("蓝牙广播停止失败: %s\n", esp_err_to_name(err));
    }
    return 0;
}

/* 注册系统级的通用诊断指令 */
static void register_system_commands(void)
{
    const esp_console_cmd_t wifi_conn_cmd = {
        .command = "wifi_connect",
        .help = "Connect to AP: wifi_connect <ssid> <password>",
        .hint = NULL,
        .func = &do_wifi_connect_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&wifi_conn_cmd));

    const esp_console_cmd_t wifi_ap_cmd = {
        .command = "wifi_ap",
        .help = "Start SoftAP: wifi_ap <ssid> [password]",
        .hint = NULL,
        .func = &do_wifi_ap_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&wifi_ap_cmd));

    const esp_console_cmd_t wifi_clear_cmd = {
        .command = "wifi_clear",
        .help = "Clear saved Wi-Fi configuration in NVS",
        .hint = NULL,
        .func = &do_wifi_clear_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&wifi_clear_cmd));

    const esp_console_cmd_t wifi_status_cmd = {
        .command = "wifi_status",
        .help = "Show Wi-Fi state and IP information",
        .hint = NULL,
        .func = &do_wifi_status_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&wifi_status_cmd));

    const esp_console_cmd_t ble_start_cmd = {
        .command = "ble_start",
        .help = "Start BLE advertising",
        .hint = NULL,
        .func = &do_ble_start_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ble_start_cmd));

    const esp_console_cmd_t ble_stop_cmd = {
        .command = "ble_stop",
        .help = "Stop BLE advertising",
        .hint = NULL,
        .func = &do_ble_stop_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ble_stop_cmd));

    const esp_console_cmd_t ui_demo_cmd = {
        .command = "ui_demo",
        .help = "Switch LVGL UI Demos: ui_demo <default|widgets|benchmark|music|stress>",
        .hint = NULL,
        .func = &do_ui_demo_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ui_demo_cmd));

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

    const esp_console_cmd_t imu_read_cmd = {
        .command = "imu_read",
        .help = "Read IMU sensors and display Pitch/Roll orientation in real-time",
        .hint = NULL,
        .func = &do_imu_read_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&imu_read_cmd));

    const esp_console_cmd_t goto_sleep_cmd = {
        .command = "goto_sleep",
        .help = "Enter sleep mode: goto_sleep <light|deep>",
        .hint = NULL,
        .func = &do_goto_sleep_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&goto_sleep_cmd));

    const esp_console_cmd_t lcd_clear_cmd = {
        .command = "lcd_clear",
        .help = "Clear screen with a color (name or hex RGB565 value)",
        .hint = NULL,
        .func = &do_lcd_clear_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&lcd_clear_cmd));

    const esp_console_cmd_t touch_read_cmd = {
        .command = "touch_read",
        .help = "Read touchscreen data and coordinates in real-time",
        .hint = NULL,
        .func = &do_touch_read_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&touch_read_cmd));
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