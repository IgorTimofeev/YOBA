#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_DESKTOP

#include <cstdint>

namespace YOBA::system {
	void delayMs(uint32_t duration);

	uint64_t getTimeUs();

	void reallocate(uint8_t*& buffer, const size_t length);
}

#endif