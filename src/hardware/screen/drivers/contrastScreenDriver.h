#pragma once

#include <cstdint>

namespace yoba {
	class ContrastScreenDriver {
		public:
			virtual void setContrast(uint8_t value) = 0;
	};
}