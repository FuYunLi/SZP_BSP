cmake_minimum_required(VERSION 3.16)

# 将自定义组件根目录注册为 ESP-IDF 组件搜索路径
set(EXTRA_COMPONENT_DIRS "app" "services" "bsp")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(word_card)
