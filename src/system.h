#pragma once

#if defined(ARDUINO)
	#if defined(ESP32) || defined(ESP8266)
		#define YOBA_SYSTEM_ARDUINO_ESP

		#include "system/mcu/arduino/esp.h"
	#endif

#elif defined(ESP32) || defined(ESP8266)
	#define YOBA_SYSTEM_ESPIDF

	#include "system/mcu/esp-idf.h"

#elif defined(WINDOWS)
	#define YOBA_SYSTEM_WINDOWS

	#include "system/desktop/windows.h"

#else
	#error Unknown platform
#endif
