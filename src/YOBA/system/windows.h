#pragma once

#include <cstdint>

namespace YOBA::system {
	void sleep(uint32_t milliseconds);

	uint32_t getTime();
}