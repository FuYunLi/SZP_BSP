idf_component_register(SRC_DIRS "wifi_service" "console"
                       INCLUDE_DIRS "wifi_service/include" "console/include"
                       REQUIRES esp_wifi esp_event lwip console esp_driver_uart)
