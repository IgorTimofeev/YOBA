#pragma once

// Arduino
#if defined(ARDUINO)
	#define YOBA_SYSTEM_MCU
	
	// ESP
	#if defined(ESP32) || defined(ESP8266)
		#define YOBA_SYSTEM_ARDUINO_ESP

		#include <YOBA/System/Arduino/ESP.hpp>
	#endif

// ESP-IDF
#elif defined(ESP_PLATFORM)
	#define YOBA_SYSTEM_MCU
	#define YOBA_SYSTEM_ESP_IDF

	#include <YOBA/System/ESP-IDF.hpp>

// Desktop
#elif defined(WIN32)
	#define YOBA_SYSTEM_DESKTOP

	#include <YOBA/System/Desktop.hpp>
#endif
