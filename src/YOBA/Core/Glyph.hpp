#pragma once

#include <cstdint>

namespace YOBA {
	class Glyph {
		public:
			constexpr Glyph() {

			}
	};

	class ConstantWidthGlyph : public Glyph {
		public:
			constexpr ConstantWidthGlyph() {

			}
	};

	class VariableWidthGlyph : public Glyph {
		public:
			constexpr VariableWidthGlyph(const uint32_t bitmapBitIndex, const uint8_t width) : _bitmapBitIndex(bitmapBitIndex), _width(width) {

			}

			constexpr uint32_t getBitmapBitIndex() const {
				return _bitmapBitIndex;
			}

			constexpr uint8_t getWidth() const {
				return _width;
			}

		private:
			const uint32_t _bitmapBitIndex;
			const uint8_t _width = 0;
	};
}