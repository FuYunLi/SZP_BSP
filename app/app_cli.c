/**
 * @file app_cli.c
 * @brief 应用层命令行接口实现，用于集中注册与管理当前项目的诊断、控制及测试命令
 */

#include "app_cli.h"
#include "app_ui.h"
#include "ble_service.h"
#include "bsp_audio.h"
#include "bsp_backlight.h"
#include "bsp_camera.h"
#include "bsp_i2c.h"
#include "bsp_key.h"
#include "bsp_lcd.h"
#include "bsp_littlefs.h"
#include "bsp_power.h"
#include "bsp_sdcard.h"
#include "bsp_touch.h"
#include "console_service.h"
#include "driver/gpio.h"
#include "esp_console.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "http_stream_server.h"
#include "img_converters.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "pca9557.h"
#include "qmi8658.h"
#include "wifi_service.h"
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

static const char *TAG = "app_cli";

/* 版本查询命令的回调函数 */
static int do_version_cmd(int argc, char **argv) {
  printf("立创实战派 S3 BSP v1.0.0\n");
  return 0;
}

/* 屏幕背光控制命令的回调函数 */
static int do_backlight_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: backlight <0|1>\n");
    return 1;
  }

  int enable = atoi(argv[1]);
  if (enable != 0 && enable != 1) {
    printf("Invalid argument. Usage: backlight <0|1>\n");
    return 1;
  }

  esp_err_t err = bsp_backlight_set(enable == 1);
  if (err != ESP_OK) {
    printf("Failed to set backlight state: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("Backlight set to %s\n", enable ? "ON" : "OFF");
  return 0;
}

/* 按键状态查询命令的回调函数 */
static int do_key_cmd(int argc, char **argv) {
  bool pressed = bsp_key_is_pressed();
  printf("BOOT Key status: %s (Level: %d)\n", pressed ? "PRESSED" : "RELEASED",
         gpio_get_level(0));
  return 0;
}

/* 进入睡眠模式诊断命令的回调函数 */
static int do_goto_sleep_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: goto_sleep <light|deep>\n");
    return 1;
  }

  const char *mode_str = argv[1];
  if (strcmp(mode_str, "light") == 0) {
    printf("Preparing to enter Light-sleep...\n");
    // 关闭屏幕背光以节省能耗
    bsp_backlight_set(false);
    // 刷新串口输出缓存以防信息截断
    fflush(stdout);
    fsync(fileno(stdout));
    vTaskDelay(pdMS_TO_TICKS(100));

    esp_err_t err = bsp_power_enter_sleep(BSP_SLEEP_MODE_LIGHT);
    if (err != ESP_OK) {
      printf("Failed to enter Light-sleep: %s\n", esp_err_to_name(err));
      bsp_backlight_set(true);
      return 1;
    }

    // 唤醒后重新开启背光并提示
    bsp_backlight_set(true);
    printf("System has woken up from Light-sleep.\n");
    return 0;
  } else if (strcmp(mode_str, "deep") == 0) {
    printf("Preparing to enter Deep-sleep...\n");
    bsp_backlight_set(false);
    fflush(stdout);
    fsync(fileno(stdout));
    vTaskDelay(pdMS_TO_TICKS(100));

    esp_err_t err = bsp_power_enter_sleep(BSP_SLEEP_MODE_DEEP);
    if (err != ESP_OK) {
      printf("Failed to enter Deep-sleep: %s\n", esp_err_to_name(err));
      bsp_backlight_set(true);
      return 1;
    }
    return 0;
  } else {
    printf("Invalid sleep mode. Usage: goto_sleep <light|deep>\n");
    return 1;
  }
}

/* LCD 清屏诊断命令的回调函数 */
static int do_lcd_clear_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("用法: lcd_clear <color_name | hex_val>\n");
    printf("可选颜色名称: red, green, blue, black, white, yellow\n");
    printf("十六进制格式: 0xF800 (RGB565)\n");
    return 1;
  }

  uint16_t color = 0x0000;
  const char *color_arg = argv[1];

  if (strcmp(color_arg, "red") == 0) {
    color = 0xF800;
  } else if (strcmp(color_arg, "green") == 0) {
    color = 0x07E0;
  } else if (strcmp(color_arg, "blue") == 0) {
    color = 0x001F;
  } else if (strcmp(color_arg, "black") == 0) {
    color = 0x0000;
  } else if (strcmp(color_arg, "white") == 0) {
    color = 0xFFFF;
  } else if (strcmp(color_arg, "yellow") == 0) {
    color = 0xFFE0;
  } else {
    char *endptr;
    long val = strtol(color_arg, &endptr, 0);
    if (endptr == color_arg || *endptr != '\0') {
      printf("无效的颜色输入: %s\n", color_arg);
      return 1;
    }
    color = (uint16_t)val;
  }

  printf("正在清屏为颜色值: 0x%04X...\n", color);
  esp_err_t err = bsp_lcd_clear(color);
  if (err != ESP_OK) {
    printf("清屏失败: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("清屏成功。\n");
  return 0;
}

/* NVS 读写测试命令的回调函数 */
static int do_nvs_test_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: nvs_test <key> [value]\n");
    return 1;
  }

  const char *key = argv[1];
  nvs_handle_t my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
    return 1;
  }

  if (argc >= 3) {
    // 写入模式
    const char *val_str = argv[2];
    char *endptr;
    long val_int = strtol(val_str, &endptr, 10);

    if (*endptr == '\0') {
      // 纯数字，写入整型 i32
      err = nvs_set_i32(my_handle, key, (int32_t)val_int);
      if (err == ESP_OK) {
        err = nvs_commit(my_handle);
      }
      if (err == ESP_OK) {
        printf("NVS Write Success: Integer %s = %ld\n", key, val_int);
      } else {
        printf("Failed to write integer: %s\n", esp_err_to_name(err));
      }
    } else {
      // 字符串，写入 str
      err = nvs_set_str(my_handle, key, val_str);
      if (err == ESP_OK) {
        err = nvs_commit(my_handle);
      }
      if (err == ESP_OK) {
        printf("NVS Write Success: String %s = \"%s\"\n", key, val_str);
      } else {
        printf("Failed to write string: %s\n", esp_err_to_name(err));
      }
    }
  } else {
    // 读取模式
    int32_t val_int = 0;
    err = nvs_get_i32(my_handle, key, &val_int);
    if (err == ESP_OK) {
      printf("Read integer: %s = %ld\n", key, (long)val_int);
    } else if (err == ESP_ERR_NVS_TYPE_MISMATCH ||
               err == ESP_ERR_NVS_NOT_FOUND) {
      size_t required_size = 0;
      err = nvs_get_str(my_handle, key, NULL, &required_size);
      if (err == ESP_OK) {
        char *buf = malloc(required_size);
        if (buf != NULL) {
          err = nvs_get_str(my_handle, key, buf, &required_size);
          if (err == ESP_OK) {
            printf("Read string: %s = \"%s\"\n", key, buf);
          } else {
            printf("Failed to read string: %s\n", esp_err_to_name(err));
          }
          free(buf);
        } else {
          printf("Memory allocation failed\n");
        }
      } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("Key '%s' not found in NVS namespace 'storage'\n", key);
      } else {
        printf("Failed to read string size: %s\n", esp_err_to_name(err));
      }
    } else {
      printf("Failed to read key '%s': %s\n", key, esp_err_to_name(err));
    }
  }

  nvs_close(my_handle);
  return 0;
}

/* I2C 总线扫描检测命令的回调函数 */
static int do_i2c_scan_cmd(int argc, char **argv) {
  i2c_master_bus_handle_t bus_handle = bsp_i2c_get_bus_handle();
  if (bus_handle == NULL) {
    printf("Error: I2C1 bus not initialized.\n");
    return 1;
  }

  printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
  for (int i = 0; i < 128; i += 16) {
    printf("%02x: ", i);
    for (int j = 0; j < 16; j++) {
      uint8_t address = i + j;
      // 0x00 - 0x02, 0x78 - 0x7f are reserved addresses in I2C specification
      if (address < 0x03 || address > 0x77) {
        printf("   ");
        continue;
      }

      esp_err_t ret = i2c_master_probe(bus_handle, address, 50);
      if (ret == ESP_OK) {
        printf("%02x ", address);
      } else {
        printf("-- ");
      }
    }
    printf("\r\n");
  }

  return 0;
}

/* PCA9557 设置引脚输出电平回调函数 */
static int do_pca9557_set_cmd(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: pca9557_set <pin> <0|1>\n");
    return 1;
  }

  int pin = atoi(argv[1]);
  int level = atoi(argv[2]);

  if (pin < 0 || pin > 7) {
    printf("Error: Pin must be 0-7.\n");
    return 1;
  }

  if (level != 0 && level != 1) {
    printf("Error: Level must be 0 or 1.\n");
    return 1;
  }

  // 先设置为输出模式
  esp_err_t err = pca9557_set_config(pin, false);
  if (err != ESP_OK) {
    printf("Error: Failed to configure pin as output: %s\n",
           esp_err_to_name(err));
    return 1;
  }

  // 设置输出电平
  err = pca9557_set_output_level(pin, level);
  if (err != ESP_OK) {
    printf("Error: Failed to set pin output level: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("PCA9557 Pin %d output level set to %d.\n", pin, level);
  return 0;
}

/* PCA9557 获取引脚状态回调函数 */
static int do_pca9557_get_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: pca9557_get <pin>\n");
    return 1;
  }

  int pin = atoi(argv[1]);
  if (pin < 0 || pin > 7) {
    printf("Error: Pin must be 0-7.\n");
    return 1;
  }

  uint8_t input_level = 0;
  uint8_t output_level = 0;

  esp_err_t err = pca9557_get_input_level(pin, &input_level);
  if (err != ESP_OK) {
    printf("Error: Failed to read input level: %s\n", esp_err_to_name(err));
    return 1;
  }

  err = pca9557_get_output_level(pin, &output_level);
  if (err != ESP_OK) {
    printf("Error: Failed to read output level: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("PCA9557 Pin %d status:\n", pin);
  printf("  Input Level  : %d\n", input_level);
  printf("  Output Level : %d (defined if configured as output)\n",
         output_level);

  return 0;
}

/* LittleFS 写入测试命令的回调函数 */
static int do_fs_write_cmd(int argc, char **argv) {
  if (!bsp_littlefs_is_mounted()) {
    printf("Error: LittleFS partition is not mounted.\n");
    return 1;
  }

  if (argc < 3) {
    printf("Usage: fs_write <file> <data>\n");
    return 1;
  }

  const char *file_name = argv[1];
  const char *data = argv[2];
  char path[128];
  snprintf(path, sizeof(path), "/littlefs/%s", file_name);

  FILE *f = fopen(path, "w");
  if (f == NULL) {
    printf("Error: Failed to open file %s for writing\n", path);
    return 1;
  }

  size_t written = fwrite(data, 1, strlen(data), f);
  fclose(f);

  if (written < strlen(data)) {
    printf("Error: Only wrote %zu of %zu bytes to %s\n", written, strlen(data),
           path);
    return 1;
  }

  printf("Successfully wrote %zu bytes to file %s\n", written, path);
  return 0;
}

/* LittleFS 读取测试命令的回调函数 */
static int do_fs_read_cmd(int argc, char **argv) {
  if (!bsp_littlefs_is_mounted()) {
    printf("Error: LittleFS partition is not mounted.\n");
    return 1;
  }

  if (argc < 2) {
    printf("Usage: fs_read <file>\n");
    return 1;
  }

  const char *file_name = argv[1];
  char path[128];
  snprintf(path, sizeof(path), "/littlefs/%s", file_name);

  FILE *f = fopen(path, "r");
  if (f == NULL) {
    printf("Error: Failed to open file %s for reading\n", path);
    return 1;
  }

  char buf[256];
  printf("Content of file %s:\n", path);
  while (fgets(buf, sizeof(buf), f) != NULL) {
    printf("%s", buf);
  }
  printf("\n");
  fclose(f);

  return 0;
}

/* MicroSD 卡目录文件列出测试命令的回调函数 */
static int do_sd_ls_cmd(int argc, char **argv) {
  if (!bsp_sdcard_is_mounted()) {
    printf("Error: SD card is not mounted.\n");
    return 1;
  }

  DIR *dir = opendir("/sdcard");
  if (dir == NULL) {
    printf("Error: Failed to open directory /sdcard\n");
    return 1;
  }

  printf("Files in /sdcard:\n");
  struct dirent *de;
  while ((de = readdir(dir)) != NULL) {
    printf("  %s\n", de->d_name);
  }
  closedir(dir);
  return 0;
}

/* MicroSD 卡文件写入测试命令的回调函数 */
static int do_sd_write_cmd(int argc, char **argv) {
  if (!bsp_sdcard_is_mounted()) {
    printf("Error: SD card is not mounted.\n");
    return 1;
  }

  if (argc < 3) {
    printf("Usage: sd_write <file> <data>\n");
    return 1;
  }

  const char *file_name = argv[1];
  const char *data = argv[2];
  char path[128];
  snprintf(path, sizeof(path), "/sdcard/%s", file_name);

  FILE *f = fopen(path, "w");
  if (f == NULL) {
    printf("Error: Failed to open file %s for writing\n", path);
    return 1;
  }

  size_t written = fwrite(data, 1, strlen(data), f);
  fclose(f);

  if (written < strlen(data)) {
    printf("Error: Only wrote %zu of %zu bytes to %s\n", written, strlen(data),
           path);
    return 1;
  }

  printf("Successfully wrote %zu bytes to file %s\n", written, path);
  return 0;
}

/* MicroSD 卡文件读取测试命令的回调函数 */
static int do_sd_read_cmd(int argc, char **argv) {
  if (!bsp_sdcard_is_mounted()) {
    printf("Error: SD card is not mounted.\n");
    return 1;
  }

  if (argc < 2) {
    printf("Usage: sd_read <file>\n");
    return 1;
  }

  const char *file_name = argv[1];
  char path[128];
  snprintf(path, sizeof(path), "/sdcard/%s", file_name);

  FILE *f = fopen(path, "r");
  if (f == NULL) {
    printf("Error: Failed to open file %s for reading\n", path);
    return 1;
  }

  char buf[256];
  printf("Content of file %s:\n", path);
  while (fgets(buf, sizeof(buf), f) != NULL) {
    printf("%s", buf);
  }
  printf("\n");
  fclose(f);
  return 0;
}

/* 6 轴姿态传感器实时读取测试命令的回调函数 */
static int do_imu_read_cmd(int argc, char **argv) {
  printf("IMU Data (Press any key to exit):\n");

  // 清空现有输入缓冲区
  fd_set readfds;
  struct timeval tv;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  while (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
    getchar();
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
  }

  while (1) {
    float acc[3];
    float gyro[3];
    float temp = 0.0f;

    esp_err_t err = qmi8658_read_parsed(acc, gyro);
    if (err == ESP_OK) {
      qmi8658_read_temp(&temp);

      // 通过重力分量计算 Pitch 与 Roll 倾角
      float pitch =
          atan2f(acc[0], sqrtf(acc[1] * acc[1] + acc[2] * acc[2])) * 57.29578f;
      float roll = atan2f(acc[1], acc[2]) * 57.29578f;

      // 实现在行首动态覆盖打印
      printf(
          "\rAcc: X=%6.3fg, Y=%6.3fg, Z=%6.3fg | Gyro: X=%6.1fdps, Y=%6.1fdps, "
          "Z=%6.1fdps | Pitch: %6.1f deg, Roll: %6.1f deg | Temp: %4.1f C",
          acc[0], acc[1], acc[2], gyro[0], gyro[1], gyro[2], pitch, roll, temp);
      fflush(stdout);
    } else {
      printf("\rError: Failed to read IMU data (%s)                   ",
             esp_err_to_name(err));
      fflush(stdout);
    }

    // 以 100ms 为周期轮询输入，如有输入则安全退出
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
      getchar(); // 吃掉该按键，避免打印多余的回车或控制台报错
      break;
    }
  }

  printf("\nExit IMU reader.\n");
  return 0;
}

/* 电容触摸屏实时数据读取测试命令的回调函数 */
static int do_touch_read_cmd(int argc, char **argv) {
  printf("Touch Panel Read (Press any key to exit):\n");
  printf("Touch the screen to cycle display colors: RED -> GREEN -> BLUE\n");

  // 清空现有输入缓冲区 (带10ms延迟等待防止输入竞态)
  fd_set readfds;
  struct timeval tv;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);
  tv.tv_sec = 0;
  tv.tv_usec = 10000;
  while (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
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

  while (1) {
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t points_num = 0;

    esp_err_t err = bsp_touch_read(&x, &y, &points_num);
    if (err == ESP_OK) {
      bool current_pressed = (points_num > 0);

      if (current_pressed) {
        printf("\rPressed: TRUE  | Points: %d | X: %3d | Y: %3d                "
               "                   ",
               points_num, x, y);

        // 触碰上升沿切换颜色
        if (!last_pressed) {
          bsp_lcd_clear(test_colors[color_idx]);
          printf("\n[Touch] Toggled LCD color to %s\n", color_names[color_idx]);
          color_idx = (color_idx + 1) % 3;
        }
      } else {
        printf("\rPressed: FALSE | Points: 0 | X: --- | Y: ---                 "
               "                  ");
      }
      fflush(stdout);
    } else {
      printf("\rError: Failed to read touch data (%s)                   ",
             esp_err_to_name(err));
      fflush(stdout);
    }

    // 以 50ms 为周期轮询输入，如果是合法字符（非回车）则安全退出
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 50000;
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
      int c = getchar();
      if (c != '\n' && c != '\r' && c != -1) {
        break;
      }
    }
  }

  printf("\nExit Touch reader.\n");
  // 测试结束清屏为黑色
  bsp_lcd_clear(0x0000);
  return 0;
}

/* 音频功放控制测试命令的回调函数 */
static int do_audio_pa_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("用法: audio_pa <0|1>\n");
    return 1;
  }

  int enable = atoi(argv[1]);
  if (enable != 0 && enable != 1) {
    printf("无效的参数。用法: audio_pa <0|1>\n");
    return 1;
  }

  esp_err_t err = bsp_audio_pa_enable(enable == 1);
  if (err != ESP_OK) {
    printf("设置音频功放失败: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("音频功放已设置为: %s\n", enable ? "开启" : "关闭");
  return 0;
}

/* 音频初始化测试命令的回调函数 */
static int do_audio_init_cmd(int argc, char **argv) {
  printf("正在初始化板载音频系统 (I2S0 & PA)...\n");
  esp_err_t err = bsp_audio_i2s_init();
  if (err != ESP_OK) {
    printf("音频初始化失败: %s\n", esp_err_to_name(err));
    return 1;
  }

  printf("音频系统初始化成功。\n");
  return 0;
}

/* 16kHz 采样率下，1kHz 正弦波的单个完整波形周期（包含 16 个立体声采样，每个采样
 * 4 字节） */
static const int16_t s_sine_lut_16k[] = {
    0,      0,      // 0度 (左/右声道相同)
    6270,   6270,   // 22.5度
    11585,  11585,  // 45度
    15137,  15137,  // 67.5度
    16384,  16384,  // 90度
    15137,  15137,  // 112.5度
    11585,  11585,  // 135度
    6270,   6270,   // 157.5度
    0,      0,      // 180度
    -6270,  -6270,  // 202.5度
    -11585, -11585, // 225度
    -15137, -15137, // 247.5度
    -16384, -16384, // 270度
    -15137, -15137, // 292.5度
    -11585, -11585, // 315度
    -6270,  -6270   // 337.5度
};

/* 音频测试发声命令的回调函数 */
static int do_audio_play_cmd(int argc, char **argv) {
  int duration_ms = 2000; // 默认播放 2 秒
  if (argc >= 2) {
    duration_ms = atoi(argv[1]);
    if (duration_ms <= 0) {
      printf("无效的播放时长参数，默认播放 2000ms\n");
      duration_ms = 2000;
    }
  }

  i2s_chan_handle_t tx_handle = bsp_audio_get_tx_handle();
  if (tx_handle == NULL) {
    printf("音频驱动未初始化，正在自动执行初始化...\n");
    esp_err_t err = bsp_audio_i2s_init();
    if (err != ESP_OK) {
      printf("自动初始化音频驱动失败: %s\n", esp_err_to_name(err));
      return 1;
    }
    tx_handle = bsp_audio_get_tx_handle();
  }

  printf("正在播放 1kHz 正弦波测试音，时长: %d ms...\n", duration_ms);

  // 1. 确保功放已开启
  bsp_audio_pa_enable(true);

  // 2. 循环写入正弦波数据
  // 16kHz 采样率下，1kHz 正弦波单个周期包含 16 个立体声点，对应 1ms 时长。
  uint32_t loops = (uint32_t)duration_ms;
  size_t bytes_written = 0;
  esp_err_t ret = ESP_OK;

  for (uint32_t i = 0; i < loops; i++) {
    ret = i2s_channel_write(tx_handle, s_sine_lut_16k, sizeof(s_sine_lut_16k),
                            &bytes_written, portMAX_DELAY);
    if (ret != ESP_OK) {
      printf("写入 I2S 通道失败: %s\n", esp_err_to_name(ret));
      break;
    }
  }

  // 3. 播放结束后，拉低功放以防底噪
  bsp_audio_pa_enable(false);

  printf("播放完成，功放已关闭。\n");
  return 0;
}

/* WAV 音频格式头定义 */
typedef struct {
  char riff_id[4];          // "RIFF"
  uint32_t riff_size;       // riff_size = file_size - 8
  char wave_id[4];          // "WAVE"
  char fmt_id[4];           // "fmt "
  uint32_t fmt_size;        // 16
  uint16_t format_tag;      // 1 (PCM)
  uint16_t channels;        // 2 (Stereo)
  uint32_t sample_rate;     // 16000
  uint32_t byte_rate;       // sample_rate * channels * bits_per_sample / 8
  uint16_t block_align;     // channels * bits_per_sample / 8
  uint16_t bits_per_sample; // 16
  char data_id[4];          // "data"
  uint32_t data_size;       // data_size = file_size - 44
} wav_header_t;

/* 麦克风录音并保存至 SD 卡指令的回调函数 */
static int do_audio_record_cmd(int argc, char **argv) {
  if (argc < 3) {
    printf("用法: audio_record <filename> <duration_sec>\n");
    printf("例如: audio_record myrecord.wav 5\n");
    return 1;
  }

  const char *file_name = argv[1];
  int duration_sec = atoi(argv[2]);
  if (duration_sec <= 0) {
    printf("录音时间必须大于 0 秒\n");
    return 1;
  }

  if (!bsp_sdcard_is_mounted()) {
    printf("错误: SD卡未挂载，无法保存录音。请先挂载 SD 卡！\n");
    return 1;
  }

  // 拼装文件路径
  char path[128];
  snprintf(path, sizeof(path), "/sdcard/%s", file_name);

  FILE *f = fopen(path, "wb");
  if (f == NULL) {
    printf("错误: 无法创建文件 %s\n", path);
    return 1;
  }

  // 写入默认的占位 WAV 文件头
  wav_header_t header = {.riff_id = {'R', 'I', 'F', 'F'},
                         .riff_size = 0,
                         .wave_id = {'W', 'A', 'V', 'E'},
                         .fmt_id = {'f', 'm', 't', ' '},
                         .fmt_size = 16,
                         .format_tag = 1, // PCM
                         .channels = 2,   // 立体声
                         .sample_rate = 16000,
                         .byte_rate = 16000 * 2 * 16 / 8,
                         .block_align = 2 * 16 / 8,
                         .bits_per_sample = 16,
                         .data_id = {'d', 'a', 't', 'a'},
                         .data_size = 0};

  if (fwrite(&header, 1, sizeof(header), f) != sizeof(header)) {
    printf("错误: 无法写入 WAV 文件头\n");
    fclose(f);
    return 1;
  }

  printf("开始录音: 目标路径 [%s], 时长 [%d 秒], 采样率 [16kHz]...\n", path,
         duration_sec);

// 每次从 I2S 读取 1024 字节的数据块并写入 SD 卡
#define RECORD_BUF_SIZE 1024
  uint8_t *buf = malloc(RECORD_BUF_SIZE);
  if (buf == NULL) {
    printf("错误: 分配录音数据内存失败\n");
    fclose(f);
    return 1;
  }

  // 计算总字节数：16000Hz * 2通道 * 16-bit(2字节) = 64000 字节每秒
  uint32_t total_bytes_expected = duration_sec * 64000;
  uint32_t bytes_written_total = 0;
  size_t bytes_read = 0;
  esp_err_t err;

  while (bytes_written_total < total_bytes_expected) {
    uint32_t bytes_to_read = total_bytes_expected - bytes_written_total;
    if (bytes_to_read > RECORD_BUF_SIZE) {
      bytes_to_read = RECORD_BUF_SIZE;
    }

    err = bsp_audio_record_read(buf, bytes_to_read, &bytes_read, 1000);
    if (err != ESP_OK) {
      printf("\n读取 I2S 音频流失败: %s\n", esp_err_to_name(err));
      break;
    }

    if (bytes_read > 0) {
      size_t written = fwrite(buf, 1, bytes_read, f);
      if (written != bytes_read) {
        printf("\n错误: 写入 SD 卡失败，可能是空间不足或卡被拔出\n");
        break;
      }
      bytes_written_total += written;

      // 打印简易百分比进度
      printf("\r录制中: %d%% (%d/%d 字节)",
             (int)(bytes_written_total * 100 / total_bytes_expected),
             (int)bytes_written_total, (int)total_bytes_expected);
      fflush(stdout);
    }
  }
  printf("\n录音结束。\n");
  free(buf);

  // 回填真正的文件大小到 WAV 文件头中
  header.riff_size = bytes_written_total + 44 - 8;
  header.data_size = bytes_written_total;

  if (fseek(f, 0, SEEK_SET) == 0) {
    fwrite(&header, 1, sizeof(header), f);
  }

  fclose(f);
  printf("成功保存录制音频到 [%s]\n", path);
  return 0;
}

/* 播放 SD 卡内 WAV 音频文件指令的回调函数 */
static int do_audio_play_wav_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("用法: audio_play_wav <filename>\n");
    printf("例如: audio_play_wav test.wav\n");
    return 1;
  }

  const char *file_name = argv[1];
  if (!bsp_sdcard_is_mounted()) {
    printf("错误: SD卡未挂载，无法播放录音。\n");
    return 1;
  }

  char path[128];
  snprintf(path, sizeof(path), "/sdcard/%s", file_name);

  FILE *f = fopen(path, "rb");
  if (f == NULL) {
    printf("错误: 无法打开文件 %s\n", path);
    return 1;
  }

  // 读取 WAV 文件头并校验
  wav_header_t header;
  if (fread(&header, 1, sizeof(header), f) != sizeof(header)) {
    printf("错误: 无法读取 WAV 文件头\n");
    fclose(f);
    return 1;
  }

  if (memcmp(header.riff_id, "RIFF", 4) != 0 ||
      memcmp(header.wave_id, "WAVE", 4) != 0) {
    printf("错误: 该文件不是有效的 WAV 格式\n");
    fclose(f);
    return 1;
  }

  printf(
      "解析 WAV 成功: 采样率 %dHz, 通道数 %d, 精度 %d-bit, 数据大小 %d 字节\n",
      (int)header.sample_rate, (int)header.channels,
      (int)header.bits_per_sample, (int)header.data_size);

  i2s_chan_handle_t tx_handle = bsp_audio_get_tx_handle();
  if (tx_handle == NULL) {
    printf("错误: I2S TX 通道未初始化\n");
    fclose(f);
    return 1;
  }

  // 使能功放
  bsp_audio_pa_enable(true);
  printf("开始播放 [%s]...\n", path);

#define PLAY_BUF_SIZE 1024
  uint8_t *buf = malloc(PLAY_BUF_SIZE);
  if (buf == NULL) {
    printf("错误: 分配播放数据缓存失败\n");
    bsp_audio_pa_enable(false);
    fclose(f);
    return 1;
  }

  uint32_t total_played = 0;
  size_t bytes_written = 0;

  // 定位到数据段起始位置（跳过 44 字节文件头）
  fseek(f, 44, SEEK_SET);

  while (total_played < header.data_size) {
    uint32_t to_read = header.data_size - total_played;
    if (to_read > PLAY_BUF_SIZE) {
      to_read = PLAY_BUF_SIZE;
    }

    size_t read_bytes = fread(buf, 1, to_read, f);
    if (read_bytes == 0) {
      break;
    }

    esp_err_t err = i2s_channel_write(tx_handle, buf, read_bytes,
                                      &bytes_written, portMAX_DELAY);
    if (err != ESP_OK) {
      printf("\n写入 I2S 发生错误: %s\n", esp_err_to_name(err));
      break;
    }

    total_played += read_bytes;
    printf("\r播放进度: %d%% (%d/%d 字节)",
           (int)(total_played * 100 / header.data_size), (int)total_played,
           (int)header.data_size);
    fflush(stdout);
  }

  printf("\n播放结束。\n");
  free(buf);
  fclose(f);

  // 播放结束后拉低功放防底噪
  bsp_audio_pa_enable(false);
  return 0;
}

/* 初始化摄像头指令的回调函数 */
static int do_camera_init_cmd(int argc, char **argv) {
  printf("开始初始化并唤醒 DVP 摄像头 (GC0308)...\n");
  esp_err_t err = bsp_camera_init();
  if (err != ESP_OK) {
    printf("错误: 摄像头初始化失败 (%s)\n", esp_err_to_name(err));
    return 1;
  }
  printf("摄像头初始化及探测成功！\n");
  return 0;
}

/* 摄像头拍照并存入 SD 卡指令的回调函数 */
static int do_camera_capture_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("用法: camera_capture <filename.bmp>\n");
    printf("例如: camera_capture snap.bmp\n");
    return 1;
  }

  const char *file_name = argv[1];
  if (!bsp_sdcard_is_mounted()) {
    printf("错误: SD卡未挂载，无法保存照片。\n");
    return 1;
  }

  // 为防此前未初始化，在此执行一次硬件初始化与唤醒
  esp_err_t err = bsp_camera_init();
  if (err != ESP_OK) {
    printf("错误: 初始化并唤醒摄像头失败\n");
    return 1;
  }

  printf("摄像头已唤醒，正在等待传感器自动曝光稳定 (500ms)...\n");
  vTaskDelay(pdMS_TO_TICKS(500));

  // 丢弃前面可能曝光不均的数帧
  camera_fb_t *fb = NULL;
  for (int i = 0; i < 3; i++) {
    fb = bsp_camera_fb_get();
    if (fb) {
      bsp_camera_fb_return(fb);
    }
  }

  printf("开始抓取图像帧...\n");
  fb = bsp_camera_fb_get();
  if (!fb) {
    printf("错误: 抓取图像帧失败，fb 为空\n");
    bsp_camera_deinit();
    return 1;
  }

  printf("图像抓取成功: 分辨率 %dx%d, 大小 %d 字节\n", (int)fb->width,
         (int)fb->height, (int)fb->len);
  printf("正在将 RGB565 转换为标准 BMP 格式...\n");

  uint8_t *bmp_buf = NULL;
  size_t bmp_len = 0;
  bool converted = frame2bmp(fb, &bmp_buf, &bmp_len);

  // 及时释放底层 FrameBuffer
  bsp_camera_fb_return(fb);

  if (!converted) {
    printf("错误: 转码 BMP 格式失败\n");
    bsp_camera_deinit();
    return 1;
  }

  char path[128];
  snprintf(path, sizeof(path), "/sdcard/%s", file_name);
  printf("正在写入 SD 卡: [%s]...\n", path);

  FILE *f = fopen(path, "wb");
  if (f == NULL) {
    printf("错误: 无法创建或打开目标文件 [%s]\n", path);
    free(bmp_buf);
    bsp_camera_deinit();
    return 1;
  }

  size_t written = fwrite(bmp_buf, 1, bmp_len, f);
  fclose(f);
  free(bmp_buf);

  // 拍照完成，恢复摄像头至休眠省电模式
  bsp_camera_deinit();

  if (written != bmp_len) {
    printf("错误: 数据写入不完整，仅写入 %d/%d 字节\n", (int)written,
           (int)bmp_len);
    return 1;
  }

  printf("成功保存照片到 [%s] (%d 字节)\n", path, (int)bmp_len);
  return 0;
}

/* 启动HTTP流媒体服务器的回调函数 */
static int do_stream_start_cmd(int argc, char **argv) {
  printf("正在启动 HTTP 流媒体服务器...\n");
  esp_err_t err = http_stream_server_start();
  if (err != ESP_OK) {
    printf("错误: HTTP 流媒体服务器启动失败 (%s)\n", esp_err_to_name(err));
    return 1;
  }
  printf("HTTP 流媒体服务器已启动！\n");
  printf("请在浏览器中访问: http://<设备IP>/stream\n");
  return 0;
}

/* 停止HTTP流媒体服务器的回调函数 */
static int do_stream_stop_cmd(int argc, char **argv) {
  printf("正在停止 HTTP 流媒体服务器...\n");
  esp_err_t err = http_stream_server_stop();
  if (err != ESP_OK) {
    printf("错误: HTTP 流媒体服务器停止失败 (%s)\n", esp_err_to_name(err));
    return 1;
  }
  printf("HTTP 流媒体服务器已停止！\n");
  return 0;
}

/* UI Demo 切换诊断命令的回调函数 */
static int do_ui_demo_cmd(int argc, char **argv) {
  if (argc < 2) {
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
static int do_wifi_connect_cmd(int argc, char **argv) {
  if (argc < 3) {
    printf("用法: wifi_connect <ssid> <password>\n");
    return 1;
  }
  const char *ssid = argv[1];
  const char *pwd = argv[2];
  printf("正在连接 Wi-Fi, SSID: %s...\n", ssid);
  esp_err_t err = wifi_service_connect(ssid, pwd);
  if (err == ESP_OK) {
    printf("连接请求已下发，请在后台观察 IP 获取情况 (可使用 wifi_status "
           "查询)。\n");
  } else {
    printf("连接启动失败: %s\n", esp_err_to_name(err));
  }
  return 0;
}

/* Wi-Fi 开启 AP 命令的回调函数 */
static int do_wifi_ap_cmd(int argc, char **argv) {
  if (argc < 2) {
    printf("用法: wifi_ap <ssid> [password]\n");
    return 1;
  }
  const char *ssid = argv[1];
  const char *pwd = (argc >= 3) ? argv[2] : NULL;
  printf("正在开启 SoftAP 热点, SSID: %s...\n", ssid);
  esp_err_t err = wifi_service_ap_start(ssid, pwd);
  if (err == ESP_OK) {
    printf("SoftAP 开启成功。\n");
  } else {
    printf("SoftAP 开启失败: %s\n", esp_err_to_name(err));
  }
  return 0;
}

/* Wi-Fi 清除配置命令的回调函数 */
static int do_wifi_clear_cmd(int argc, char **argv) {
  printf("正在清除 NVS 中的 Wi-Fi 配置...\n");
  esp_err_t err = wifi_service_clear_config();
  if (err == ESP_OK) {
    printf("Wi-Fi 配置已成功清除。\n");
  } else {
    printf("配置清除失败: %s\n", esp_err_to_name(err));
  }
  return 0;
}

/* Wi-Fi 状态查询命令的回调函数 */
static int do_wifi_status_cmd(int argc, char **argv) {
  wifi_service_state_t state = wifi_service_get_state();
  const char *state_str = "UNKNOWN";
  switch (state) {
  case WIFI_SERVICE_STATE_DISCONNECTED:
    state_str = "DISCONNECTED";
    break;
  case WIFI_SERVICE_STATE_CONNECTING:
    state_str = "CONNECTING";
    break;
  case WIFI_SERVICE_STATE_CONNECTED:
    state_str = "CONNECTED (WAITING FOR IP)";
    break;
  case WIFI_SERVICE_STATE_GOT_IP:
    state_str = "CONNECTED (GOT IP)";
    break;
  case WIFI_SERVICE_STATE_FAILED:
    state_str = "FAILED";
    break;
  }
  printf("Wi-Fi 连接状态: %s\n", state_str);

  if (state == WIFI_SERVICE_STATE_GOT_IP) {
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
      printf("IP 地址  : " IPSTR "\n", IP2STR(&ip_info.ip));
      printf("子网掩码 : " IPSTR "\n", IP2STR(&ip_info.netmask));
      printf("网关     : " IPSTR "\n", IP2STR(&ip_info.gw));
    }
  }
  return 0;
}

/* BLE 广播启动命令的回调函数 */
static int do_ble_start_cmd(int argc, char **argv) {
  printf("正在启动 BLE 蓝牙广播...\n");
  esp_err_t err = ble_service_start();
  if (err == ESP_OK) {
    printf("蓝牙广播已成功启动。\n");
  } else {
    printf("蓝牙广播启动失败: %s\n", esp_err_to_name(err));
  }
  return 0;
}

/* BLE 广播停止命令的回调函数 */
static int do_ble_stop_cmd(int argc, char **argv) {
  printf("正在停止 BLE 蓝牙广播...\n");
  esp_err_t err = ble_service_stop();
  if (err == ESP_OK) {
    printf("蓝牙广播已停止。\n");
  } else {
    printf("蓝牙广播停止失败: %s\n", esp_err_to_name(err));
  }
  return 0;
}

/* 注册系统级的通用诊断指令 */
static void register_system_commands(void) {
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
      .help = "Switch LVGL UI Demos: ui_demo "
              "<default|widgets|benchmark|music|stress>",
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
      .help =
          "Read IMU sensors and display Pitch/Roll orientation in real-time",
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

  const esp_console_cmd_t audio_pa_cmd = {
      .command = "audio_pa",
      .help = "Control Audio PA: audio_pa <0|1>",
      .hint = NULL,
      .func = &do_audio_pa_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&audio_pa_cmd));

  const esp_console_cmd_t audio_init_cmd = {
      .command = "audio_init",
      .help = "Initialize Board Audio (I2S0 & PA)",
      .hint = NULL,
      .func = &do_audio_init_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&audio_init_cmd));

  const esp_console_cmd_t audio_play_cmd = {
      .command = "audio_play",
      .help = "Play 1kHz sine wave sound: audio_play [duration_ms]",
      .hint = NULL,
      .func = &do_audio_play_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&audio_play_cmd));

  const esp_console_cmd_t audio_record_cmd = {
      .command = "audio_record",
      .help =
          "Record MIC sound to SD card: audio_record <filename> <duration_sec>",
      .hint = NULL,
      .func = &do_audio_record_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&audio_record_cmd));

  const esp_console_cmd_t audio_play_wav_cmd = {
      .command = "audio_play_wav",
      .help = "Play WAV sound from SD card: audio_play_wav <filename>",
      .hint = NULL,
      .func = &do_audio_play_wav_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&audio_play_wav_cmd));

  const esp_console_cmd_t camera_init_cmd = {
      .command = "camera_init",
      .help = "Initialize and probe Board DVP Camera (GC0308)",
      .hint = NULL,
      .func = &do_camera_init_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&camera_init_cmd));

  const esp_console_cmd_t camera_capture_cmd = {
      .command = "camera_capture",
      .help = "Capture camera frame to SD card as BMP: camera_capture "
              "<filename.bmp>",
      .hint = NULL,
      .func = &do_camera_capture_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&camera_capture_cmd));

  const esp_console_cmd_t stream_start_cmd = {
      .command = "stream_start",
      .help = "Start HTTP stream server for MJPEG video streaming",
      .hint = NULL,
      .func = &do_stream_start_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&stream_start_cmd));

  const esp_console_cmd_t stream_stop_cmd = {
      .command = "stream_stop",
      .help = "Stop HTTP stream server",
      .hint = NULL,
      .func = &do_stream_stop_cmd,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&stream_stop_cmd));
}

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 初始化并注册应用层的控制台命令
 */
esp_err_t app_cli_init(void) {
  ESP_LOGI(TAG, "Initializing app CLI...");

  ESP_ERROR_CHECK(console_service_start());

  // 注册自定义的命令
  register_system_commands();

  return ESP_OK;
}