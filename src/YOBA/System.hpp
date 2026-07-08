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

// Windows
#elif defined(WIN32)
	#define YOBA_SYSTEM_DESKTOP
	#define YOBA_SYSTEM_WINDOWS
	
	#include <YOBA/System/Windows.hpp>
#endif