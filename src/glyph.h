#pragma once

#include "cstdlib"

namespace yoba {
	class Glyph {
		public:
			Glyph(uint32_t bitmapBitIndex, uint16_t width);

			uint32_t getBitmapBitIndex() const;
			uint8_t getWidth() const;

		private:
			const uint32_t _bitmapBitIndex = 0;
			const uint8_t _width;
	};
}