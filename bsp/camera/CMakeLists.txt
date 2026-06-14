idf_component_register(SRCS "bsp_camera.c"
                      INCLUDE_DIRS "include"
                      REQUIRES pca9557 esp32-camera i2c)
