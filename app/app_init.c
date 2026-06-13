/**
 * @file app_init.c
 * @brief 应用初始化入口实现，负责系统服务与上层业务的组装与启动
 */

#include "app_init.h"
#include "app_cli.h"
#include "bsp_backlight.h"

/* ================================================================
 * 公开接口实现
 * ================================================================ */

/**
 * @brief 系统应用初始化入口函数
 */
void app_init(void)
{
    // 初始化板载背光硬件
    ESP_ERROR_CHECK(bsp_backlight_init());

    // 启动调试控制台及应用层命令行接口
    ESP_ERROR_CHECK(app_cli_init());
}
