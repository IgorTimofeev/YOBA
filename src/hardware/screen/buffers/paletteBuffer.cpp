#include "paletteBuffer.h"

namespace yoba {
	PaletteBuffer::PaletteBuffer(ScreenDriver *driver, uint16_t *palette) : ScreenBuffer(driver), _palette(palette) {

	}

	uint8_t PaletteBuffer::getPaletteIndex(const Color *color) {
		// TODO: implement other color types
		switch (color->getType()) {
			case ColorType::Palette:
				return ((ColorPalette*) color)->getIndex();

			case ColorType::Bit24: {
				auto color24 = (Color24*) color;

				return ColorPalette::getClosestIndex(_palette, 256, *color24);
			}

			default:
				return 0;
		}
	}

	void PaletteBuffer::clearUnchecked(const Color* color) {
		memset((uint16_t*) this->_buffer, (int) getPaletteIndex(color), this->_bufferLength);
	}

	uint16_t PaletteBuffer::getPaletteColor(uint8_t index) {
		return _palette[index];
	}

	void PaletteBuffer::setPaletteColor(uint8_t index, uint16_t value) {
		_palette[index] = value;
	}
}