#pragma once

#include <cstdint>

namespace YOBA {
	class DirectRenderTarget {
		public:
			virtual void writePixels(uint8_t* source) = 0;
	};
}