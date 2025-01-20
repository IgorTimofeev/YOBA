#pragma once

#if defined(ARDUINO)
	#if defined(ESP32)
		#include "system/mcu/arduino/esp32.h"
	#endif

#elif defined(WINDOWS)
	#define YOBA_SYSTEM_WINDOWS


#endif
