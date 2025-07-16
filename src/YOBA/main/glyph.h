#pragma once

#include "cstdlib"

namespace YOBA {
	class Glyph {
		public:
			constexpr explicit Glyph(const uint32_t bitmapIndex) :  bitmapIndex(bitmapIndex) {

			}

			uint32_t getBitmapIndex() const {
				return bitmapIndex;
			}

		private:
			uint32_t bitmapIndex;
	};

	class VariableWidthGlyph : public Glyph {
		public:
			constexpr VariableWidthGlyph(const uint32_t bitmapIndex, const uint8_t width) : Glyph(bitmapIndex), width(width) {

			}

			uint8_t getWidth() const {
				return width;
			}

		private:
			const uint8_t width = 0;
	};
}