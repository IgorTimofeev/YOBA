#include "eightBitsPaletteBuffer.h"
#include "bounds.h"

namespace yoba {
	EightBitsPaletteBuffer::EightBitsPaletteBuffer(ScreenDriver *driver) : PaletteBuffer(driver, _govnoPalette) {

	}

	void EightBitsPaletteBuffer::allocate() {
		_bufferLength = getDriver()->getSize().getWidth() * getDriver()->getSize().getHeight();
		_buffer = new uint8_t[_bufferLength];
	}

	void EightBitsPaletteBuffer::flush() {
		const size_t pixelCount = getDriver()->getSize().getWidth() * _driver->getTransactionBufferHeight();
		size_t bufferIndex = 0;

		for (uint16_t y = 0; y < getDriver()->getSize().getHeight(); y += _driver->getTransactionBufferHeight()) {
			for (size_t i = 0; i < pixelCount; i++) {
				_driver->getTransactionBuffer()[i] = _palette[_buffer[bufferIndex]];
				bufferIndex++;
			}

			_driver->flushTransactionBuffer(y);
		}
	}

	void EightBitsPaletteBuffer::renderPixelUnchecked(const Point &point, const Color* color) {
		_buffer[getIndex(point)] = getPaletteIndex(color);
	}

	void EightBitsPaletteBuffer::renderHorizontalLineUnchecked(const Point &point, uint16_t width, const Color* color) {
		memset(_buffer + getIndex(point), getPaletteIndex(color), width);
	}

	void EightBitsPaletteBuffer::renderVerticalLineUnchecked(const Point &point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(point);
		uint16_t scanlineLength = getDriver()->getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void EightBitsPaletteBuffer::renderFilledRectangleUnchecked(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = getDriver()->getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void EightBitsPaletteBuffer::fillWithOpenComputersColors() {
		const uint8_t reds = 6;
		const uint8_t greens = 8;
		const uint8_t blues = 5;

		for (uint8_t index = 0; index < 240; index++) {
			const uint8_t idxB = index % blues;
			const uint8_t idxG = (index / blues) % greens;
			const uint8_t idxR = (index / blues / greens) % reds;

			const auto r = (uint8_t) round((float) idxR * 255.0f / ((float) reds - 1.0f));
			const auto g = (uint8_t) round((float) idxG * 255.0f / ((float) greens - 1.0f));
			const auto b = (uint8_t) round((float) idxB * 255.0f / ((float) blues - 1.0f));

			_palette[index] = Color24(r, g, b).toUint16();
		}

		for (uint8_t index = 0; index < 16; index++) {
			const auto shade = (uint8_t) round(255.0f * (float) (index + 1) / 16.0f);

			_palette[240 + index] = Color24(shade, shade, shade).toUint16();
		}
	}
}