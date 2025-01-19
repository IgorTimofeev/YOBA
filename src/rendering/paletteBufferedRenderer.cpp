#include "paletteBufferedRenderer.h"

namespace yoba {
	PaletteBufferedRenderer::PaletteBufferedRenderer(uint16_t paletteLength) : _paletteLength(paletteLength) {

	}




	uint32_t PaletteBufferedRenderer::getPaletteColor(uint16_t index) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				return ((uint16_t*) _palette)[index];

			case ColorModel::rgb666:
				return ((uint32_t*) (_palette + index * 3))[0];

			default:
				return 0;
		}
	}

	void PaletteBufferedRenderer::setPaletteColor(uint16_t index, uint32_t value) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565: {
				((uint16_t*) _palette)[index] = (uint16_t) value;
				break;
			}

			case ColorModel::rgb666: {
				const auto palettePtr = _palette + index * 3;
				const auto valuePtr = (uint8_t*) &value;
				palettePtr[0] = valuePtr[2];
				palettePtr[1] = valuePtr[1];
				palettePtr[2] = valuePtr[0];

				break;
			}

			default:
				_palette[index] = 0;
				break;
		}
	}

	uint16_t PaletteBufferedRenderer::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::palette:
				return ((PaletteColor*) color)->getIndex();

			default:
				return 0;
		}
	}

	void PaletteBufferedRenderer::setPaletteColor(uint16_t index, const Rgb888Color& color) {
		uint32_t tColor;

		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				tColor = color.toRgb565().getValue();
				break;

			case ColorModel::rgb666:
				tColor = color.toRgb666().getValue();
				break;

			default:
				tColor = 0;
				break;
		}

		setPaletteColor(index, tColor);
	}

	void PaletteBufferedRenderer::setPaletteColors(std::initializer_list<Rgb888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	void PaletteBufferedRenderer::setPaletteColors(std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	uint16_t PaletteBufferedRenderer::getPaletteLength() const {
		return _paletteLength;
	}

	uint8_t* PaletteBufferedRenderer::getPalette() const {
		return _palette;
	}

	void PaletteBufferedRenderer::onTargetChanged() {
		BufferedRenderer::onTargetChanged();

		// (Re)allocating palette
		delete _palette;

		if (getTarget()) {
			delete _palette;
			_palette = (uint8_t*) malloc(_paletteLength * Color::getBytesPerType(getTarget()->getColorModel()));
			assert(!!_palette);
		}
	}
}