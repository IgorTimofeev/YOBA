#include "glyph.h"

namespace yoba {
	Glyph::Glyph(uint32_t bitmapBitIndex, uint16_t width) : _bitmapBitIndex(bitmapBitIndex), _width(width) {

	}

	uint32_t Glyph::getBitmapBitIndex() const {
		return _bitmapBitIndex;
	}

	uint8_t Glyph::getWidth() const {
		return _width;
	}
}
