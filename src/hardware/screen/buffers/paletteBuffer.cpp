#include "paletteBuffer.h"

namespace yoba {
	PaletteBuffer::PaletteBuffer(ScreenDriver* driver, uint16_t paletteLength) : ScreenBuffer(driver), _paletteLength(paletteLength) {

	}

	void PaletteBuffer::allocate() {
		ScreenBuffer::allocate();

		_palette = (uint8_t*) malloc(_paletteLength * Color::getBytesPerType(_driver->getColorModel()));
		assert(_palette != nullptr);
	}

	uint32_t PaletteBuffer::getPaletteColor(uint16_t index) {
		switch (_driver->getColorModel()) {
			case ColorModel::Rgb565:
				return ((uint16_t*) _palette)[index];

			case ColorModel::Rgb666:
				return ((uint32_t*) _palette)[index];

			default:
				return 0;
		}
	}

	void PaletteBuffer::setPaletteColor(uint16_t index, uint32_t value) {
		switch (_driver->getColorModel()) {
			case ColorModel::Rgb565:
				((uint16_t*) _palette)[index] = value;
				break;

			case ColorModel::Rgb666:
				((uint32_t*) _palette)[index] = value;
				break;

			default:
				_palette[index] = 0;
				break;
		}
	}

	uint16_t PaletteBuffer::getPaletteIndex(const Color *color) {
		switch (color->getModel()) {
			case ColorModel::Palette:
				return ((PaletteColor*) color)->getIndex();

			default:
				return 0;
		}
	}

	void PaletteBuffer::setPaletteColor(uint16_t index, const Rgb888Color &color) {
		uint32_t tColor;

		switch (_driver->getColorModel()) {
			case ColorModel::Rgb565:
				tColor = color.toRgb565().getValue();
				break;

			case ColorModel::Rgb666:
				tColor = color.toRgb666().getValue();
				break;

			default:
				tColor = 0;
				break;
		}

		setPaletteColor(index, tColor);
	}

	void PaletteBuffer::setPaletteColors(std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (auto color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}
}