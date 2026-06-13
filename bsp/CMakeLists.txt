idf_component_register(SRC_DIRS "key" "backlight" "i2c" "littlefs" "sdcard" "power"
                       INCLUDE_DIRS "key/include" "backlight/include" "i2c/include" "littlefs/include" "sdcard/include" "power/include"
                       REQUIRES driver esp_event fatfs sdmmc esp_pm)
