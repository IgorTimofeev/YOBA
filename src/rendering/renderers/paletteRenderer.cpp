#include "paletteRenderer.h"

namespace yoba {
	PaletteRenderer::PaletteRenderer(uint16_t paletteLength) : _paletteLength(paletteLength) {

	}


	uint32_t PaletteRenderer::getPaletteValue(uint16_t index) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				return ((uint16_t*) _palette)[index];

			case ColorModel::rgb666:
				return ((uint32_t*) (_palette + index * 3))[0];

			default:
				return 0;
		}
	}

	void PaletteRenderer::setPaletteValue(uint16_t index, uint32_t value) {
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

	uint16_t PaletteRenderer::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::palette:
				return ((PaletteColor*) color)->getIndex();

			default:
				return 0;
		}
	}

	void PaletteRenderer::setPaletteColor(uint16_t index, const Rgb888Color& color) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				setPaletteValue(index, color.toRgb565().getValue());
				break;

			case ColorModel::rgb666:
				setPaletteValue(index, color.toRgb666().getValue());
				break;

			default:
				setPaletteValue(index, 0);
				break;
		}
	}

	void PaletteRenderer::setPaletteColors(std::initializer_list<Rgb888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	void PaletteRenderer::setPaletteColors(std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}

	uint16_t PaletteRenderer::getPaletteLength() const {
		return _paletteLength;
	}

	uint8_t* PaletteRenderer::getPalette() const {
		return _palette;
	}

	void PaletteRenderer::onTargetChanged() {
		Renderer::onTargetChanged();

		// (Re)allocating palette
		delete _palette;

		if (getTarget()) {
			delete _palette;
			_palette = (uint8_t*) malloc(_paletteLength * Color::getBytesPerModel(getTarget()->getColorModel()));
			assert(!!_palette);
		}
	}
}