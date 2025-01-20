#pragma once

#include <cstdint>

namespace yoba::system {
	void sleep(uint32_t milliseconds);

	uint32_t getTime();
}