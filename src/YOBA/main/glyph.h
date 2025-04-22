#pragma once

#include "cstdlib"

namespace YOBA {
	class Glyph {
		public:
			constexpr explicit Glyph(uint32_t bitmapIndex) :  _bitmapIndex(bitmapIndex) {

			}

			uint32_t getBitmapIndex() const {
				return _bitmapIndex;
			}

		private:
			uint32_t _bitmapIndex;
	};

	class VariableWidthGlyph : public Glyph {
		public:
			constexpr VariableWidthGlyph(uint32_t bitmapIndex, const uint8_t width) : Glyph(bitmapIndex), _width(width) {

			}

			uint8_t getWidth() const {
				return _width;
			}

		private:
			const uint8_t _width = 0;
	};
}