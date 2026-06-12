#include "app_init.h"
#include "console_service.h"

void app_init(void)
{
    // 启动调试控制台服务
    ESP_ERROR_CHECK(console_service_start());
}
