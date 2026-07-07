cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
    file(GLOB_RECURSE YOBA_SOURCES
        "src/*.*"
    )

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
endif()