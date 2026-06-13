idf_component_register(SRC_DIRS "key" "backlight" "i2c" "littlefs"
                       INCLUDE_DIRS "key/include" "backlight/include" "i2c/include" "littlefs/include"
                       REQUIRES driver esp_event)
