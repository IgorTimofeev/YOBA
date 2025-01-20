#pragma once

#ifndef YOBA_SYSTEM
	#define YOBA_SYSTEM

	#if defined(ARDUINO)
		#define YOBA_SYSTEM_MCU

		#if defined(ESP32)
			#include "system/mcu/arduino/esp32.h"
		#endif
	#elif defined(WINDOWS)
		#define YOBA_SYSTEM_WINDOWS


	#endif
#endif
