idf_component_register(SRC_DIRS "key" "backlight" "i2c" "littlefs" "sdcard"
                       INCLUDE_DIRS "key/include" "backlight/include" "i2c/include" "littlefs/include" "sdcard/include"
                       REQUIRES driver esp_event fatfs sdmmc)
