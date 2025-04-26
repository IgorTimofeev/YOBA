#include "paletteBufferRenderer.h"

namespace YOBA {
	PaletteBufferRenderer::PaletteBufferRenderer(uint16_t paletteColorCount) : _paletteColorCount(paletteColorCount) {

	}

	void PaletteBufferRenderer::updateFromTarget() {
		TransactionBufferRenderer::updateFromTarget();

		reallocatePaletteBuffer();
		reallocatePalette();
	}

	void PaletteBufferRenderer::reallocatePaletteBuffer() {
		if (!getTarget())
			return;

		delete _paletteBuffer;

		_paletteBufferLength = computePaletteBufferLengthForTarget();
		_paletteBuffer = new uint8_t[_paletteBufferLength];
	}

	void PaletteBufferRenderer::reallocatePalette() {
		if (!getTarget())
			return;

		delete _palette;
		_palette = new uint8_t[_paletteColorCount * Color::getBytesPerModel(getTarget()->getColorModel())];
	}

	uint32_t PaletteBufferRenderer::getPaletteValue(uint16_t index) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				return ((uint16_t*) _palette)[index];

			case ColorModel::rgb666:
				return ((uint32_t*) (_palette + index * 3))[0];

			default:
				return 0;
		}
	}

	void PaletteBufferRenderer::setPaletteValue(uint16_t index, uint32_t value) {
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

	uint16_t PaletteBufferRenderer::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::palette:
				return ((PaletteColor*) color)->getIndex();

			default:
				return 0;
		}
	}

	void PaletteBufferRenderer::setPaletteColor(uint16_t index, const Rgb888Color& color) {
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

	void PaletteBufferRenderer::setPaletteColors(std::initializer_list<Rgb888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	void PaletteBufferRenderer::setPaletteColors(std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}

	uint16_t PaletteBufferRenderer::getPaletteColorCount() const {
		return _paletteColorCount;
	}

	uint8_t* PaletteBufferRenderer::getPalette() const {
		return _palette;
	}

	uint8_t* PaletteBufferRenderer::getPaletteBuffer() const {
		return _paletteBuffer;
	}

	size_t PaletteBufferRenderer::getPaletteBufferLength() const {
		return _paletteBufferLength;
	}

	size_t PaletteBufferRenderer::getPaletteBufferIndex(uint16_t x, uint16_t y) const {
		return y * getTarget()->getSize().getWidth() + x;
	}

	size_t PaletteBufferRenderer::getPaletteBufferIndex(const Point& point) const {
		return getPaletteBufferIndex(point.getX(), point.getY());
	}
}