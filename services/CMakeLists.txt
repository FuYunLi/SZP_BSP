idf_component_register(SRC_DIRS "wifi_service" "console" "ble_service" "http_stream_service"
                       INCLUDE_DIRS "wifi_service/include" "console/include" "ble_service/include" "http_stream_service/include"
                       REQUIRES esp_wifi esp_event lwip console esp_driver_uart bt nvs_flash esp_http_server)
