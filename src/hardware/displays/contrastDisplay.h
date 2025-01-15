#pragma once

#include <cstdint>

namespace yoba::hardware {
	class ContrastDisplay {
		public:
			virtual void setContrast(uint8_t value) = 0;
	};
}