cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE YOBA_SOURCES "src/*.cpp")

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${YOBA_SOURCES}
        INCLUDE_DIRS src
        REQUIRES
            esp_timer
            esp_driver_spi
            esp_driver_gpio
            esp_driver_i2c
    )

    return()
else()
    project(YOBA)

    add_library(YOBA ${YOBA_SOURCES})
    target_include_directories(YOBA PUBLIC src)
endif()