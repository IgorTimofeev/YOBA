#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_WINDOWS)

#pragma once

#include <cstdint>

namespace YOBA::system {
	void delayMs(uint32_t duration);

	uint64_t getTimeUs();

	void reallocate(uint8_t*& buffer, const size_t length);
}

#endif