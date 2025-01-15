#pragma once

#include <cstdint>

namespace yoba::hardware {
	class DirectWritingDisplay {
		public:
			virtual void writePixels(uint8_t* source) = 0;
	};
}