#pragma once

// ESP-IDF
#if defined(ESP_PLATFORM)
	#define YOBA_SYSTEM_MCU
	#define YOBA_SYSTEM_GPIO
	#define YOBA_SYSTEM_SPI
	#define YOBA_SYSTEM_I2C
	#define YOBA_SYSTEM_ESP_IDF

	#include <YOBA/System/ESP-IDF.hpp>

// Arduino
#elif defined(ARDUINO)
	#define YOBA_SYSTEM_MCU
	#define YOBA_SYSTEM_GPIO
	#define YOBA_SYSTEM_SPI
	#define YOBA_SYSTEM_I2C

	// ESP
	#if defined(ESP32) || defined(ESP8266)
		#define YOBA_SYSTEM_ARDUINO_ESP

		#include <YOBA/System/Arduino/ESP.hpp>
	#endif
// Desktop
#elif defined(WIN32)
	#define YOBA_SYSTEM_DESKTOP
	#define YOBA_SYSTEM_SFML

	#include <YOBA/System/Desktop.hpp>
#endif