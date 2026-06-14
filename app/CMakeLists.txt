idf_component_register(SRCS "app_cli.c" "app_init.c" "app_ui.c" "http_stream_server.c"
                       INCLUDE_DIRS "include"
                       REQUIRES bsp services nvs_flash pca9557 qmi8658 esp_http_server espressif__esp_new_jpeg)
