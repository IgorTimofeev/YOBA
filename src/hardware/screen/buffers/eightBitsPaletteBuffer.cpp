#include "eightBitsPaletteBuffer.h"
#include "bounds.h"

namespace yoba {
	EightBitsPaletteBuffer::EightBitsPaletteBuffer(ScreenDriver *driver, const Size& resolution) : PaletteBuffer<uint8_t>(driver, resolution, _govnoPalette) {

	}

	void EightBitsPaletteBuffer::allocate() {
		_bufferLength = getSize().getWidth() * getSize().getHeight();
		_buffer = new uint8_t[_bufferLength];
	}

	void EightBitsPaletteBuffer::flush() {
		const size_t pixelCount = getSize().getWidth() * _driver->getTransactionBufferHeight();
		size_t bufferIndex = 0;

		for (uint16_t y = 0; y < getSize().getHeight(); y += _driver->getTransactionBufferHeight()) {
			for (size_t i = 0; i < pixelCount; i++) {
				_driver->getTransactionBuffer()[i] = _palette[_buffer[bufferIndex]];
				bufferIndex++;
			}

			_driver->flushTransactionBuffer(this, y);
		}
	}

	uint8_t EightBitsPaletteBuffer::getNativeColor(const Color* color) {
		switch (color->getType()) {
			case ColorType::Palette:
				return ((ColorPalette*) color)->getIndex();

			// TODO: implement other color types
			default:
				return 0;
		}
	}

	void EightBitsPaletteBuffer::renderPixelNative(const Point &point, uint8_t paletteIndex) {
		_buffer[getIndex(point)] = paletteIndex;
	}

	void EightBitsPaletteBuffer::renderHorizontalLineNative(const Point &point, uint16_t width, uint8_t paletteIndex) {
		memset(_buffer + getIndex(point), paletteIndex, width);
	}

	void EightBitsPaletteBuffer::renderVerticalLineNative(const Point &point, uint16_t height, uint8_t paletteIndex) {
		uint8_t* bufferPtr = _buffer + getIndex(point);
		uint16_t scanlineLength = getSize().getWidth();

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void EightBitsPaletteBuffer::renderFilledRectangleNative(const Bounds& bounds, uint8_t paletteIndex) {
		uint8_t* bufferPtr = _buffer + getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = getSize().getWidth();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}
}