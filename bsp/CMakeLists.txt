idf_component_register(SRC_DIRS "key" "backlight" "i2c" "littlefs" "sdcard" "power" "lcd" "touch"
                       INCLUDE_DIRS "key/include" "backlight/include" "i2c/include" "littlefs/include" "sdcard/include" "power/include" "lcd/include" "touch/include"
                       REQUIRES driver esp_event fatfs sdmmc esp_pm esp_lcd pca9557 esp_lcd_touch esp_lcd_touch_ft5x06)
