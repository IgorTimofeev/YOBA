#pragma once

#if defined(ARDUINO)
	#if defined(ESP32) || defined(ESP8266)
		#define YOBA_SYSTEM_ARDUINO_ESP

		#include <YOBA/System/Arduino/ESP32.hpp>
	#endif

#elif defined(ESP_PLATFORM)
	#define YOBA_SYSTEM_ESP_IDF

	#include <YOBA/System/ESP-IDF.hpp>

#elif defined(WINDOWS)
	#define YOBA_SYSTEM_WINDOWS

	#include <YOBA/System/Windows.hpp>

#else
	#error Unknown platform
#endif
