#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_DESKTOP

#include <cstdint>
#include <chrono>

namespace YOBA {
	class Timer {
		public:
			static void delayMs(uint32_t duration);

			static uint64_t getTimeUs();
	};

	class Memory {
		public:
			static void reallocate(uint8_t*& buffer, const size_t length);
	};
}

#endif