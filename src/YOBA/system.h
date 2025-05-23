#pragma once

#if defined(ARDUINO)
	#if defined(ESP32) || defined(ESP8266)
		#define YOBA_SYSTEM_ARDUINO_ESP

		#include <YOBA/system/arduino/ESP32.h>
	#endif

#elif defined(ESP_PLATFORM)
	#define YOBA_SYSTEM_ESPIDF

	#include <YOBA/system/ESP-IDF.h>

#elif defined(WINDOWS)
	#define YOBA_SYSTEM_WINDOWS

	#include <YOBA/system/windows.h>

#else
	#error Unknown platform
#endif
