#pragma once

// ESP-IDF
#if defined(ESP_PLATFORM)
	#include <esp_heap_caps.h>

	#define YOBA_SYSTEM_MCU
	#define YOBA_SYSTEM_GPIO
	#define YOBA_SYSTEM_SPI
	#define YOBA_SYSTEM_I2C
	#define YOBA_SYSTEM_ESP_IDF

	#ifdef CONFIG_SPIRAM
		#define YOBA_SYSTEM_PSRAM
	#endif

	#include <YOBA/System/ESP-IDF.hpp>

// Arduino
#elif defined(ARDUINO)
	#define YOBA_SYSTEM_MCU
	#define YOBA_SYSTEM_GPIO
	#define YOBA_SYSTEM_SPI
	#define YOBA_SYSTEM_I2C

	// ESP
	#if defined(ESP32) || defined(ESP8266)
		#include <esp_heap_caps.h>

		#define YOBA_SYSTEM_ARDUINO_ESP

		#ifdef CONFIG_SPIRAM
			#define YOBA_SYSTEM_PSRAM
		#endif

		#include <YOBA/System/Arduino/ESP.hpp>
	#endif

// Desktop
#elif defined(WIN32)
	#define YOBA_SYSTEM_DESKTOP

	#include <YOBA/System/Desktop.hpp>
#endif

// SFML
#if __has_include(<SFML/Config.hpp>)
	#define YOBA_SYSTEM_SFML
#endif