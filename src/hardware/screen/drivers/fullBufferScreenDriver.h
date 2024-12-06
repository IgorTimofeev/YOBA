#pragma once

namespace yoba {
	class FullBufferScreenDriver {
		public:
			virtual void writePixels(uint8_t* buffer) = 0;
	};
}