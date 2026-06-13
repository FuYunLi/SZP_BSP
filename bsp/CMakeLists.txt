idf_component_register(SRC_DIRS "key" "backlight" "i2c" "littlefs" "sdcard" "power" "lcd"
                       INCLUDE_DIRS "key/include" "backlight/include" "i2c/include" "littlefs/include" "sdcard/include" "power/include" "lcd/include"
                       REQUIRES driver esp_event fatfs sdmmc esp_pm esp_lcd pca9557)
