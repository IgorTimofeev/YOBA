#pragma once

#include <cstdint>

namespace yoba::hardware {
	class DirectRenderTarget {
		public:
			virtual void writePixels(uint8_t* source) = 0;
	};
}