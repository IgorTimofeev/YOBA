#include "bits8PaletteBuffer.h"
#include "bounds.h"

namespace yoba {
	Bits8PaletteBuffer::Bits8PaletteBuffer(ScreenDriver *driver) : PaletteBuffer(driver) {

	}

	void Bits8PaletteBuffer::allocate() {
		_bufferLength = getDriver()->getResolution().getWidth() * getDriver()->getResolution().getHeight();
		_buffer = new uint8_t[_bufferLength];
	}

	void Bits8PaletteBuffer::flush() {
		_driver->writePixelData([&](size_t pixelIndex) {
			return _palette[_buffer[pixelIndex]];
		});
	}

	void Bits8PaletteBuffer::renderPixelNative(const Point& point, const Color* color) {
		_buffer[getIndex(point)] = getPaletteIndexOf(color);
	}

	void Bits8PaletteBuffer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(_buffer + getIndex(point), getPaletteIndexOf(color), width);
	}

	void Bits8PaletteBuffer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(point);
		uint16_t scanlineLength = getDriver()->getResolution().getWidth();
		auto paletteIndex = getPaletteIndexOf(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void Bits8PaletteBuffer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = getDriver()->getResolution().getWidth();
		auto paletteIndex = getPaletteIndexOf(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void Bits8PaletteBuffer::renderImageNative(const Point& point, const Image* image) {
		size_t
			bufferIndex = getIndex(point),
			scanlineLength = getDriver()->getResolution().getWidth() - image->getSize().getWidth(),
			imageIndex = 0;

		for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
			for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
				_buffer[bufferIndex] = image->getBitmap()[imageIndex];

				bufferIndex++;
				imageIndex++;
			}

			bufferIndex += scanlineLength;
		}
	}

	void Bits8PaletteBuffer::setOpenComputersPaletteColors() {
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

			_palette[index] = Rgb888Color(r, g, b).toRgb565().getValue();
		}

		for (uint8_t index = 0; index < 16; index++) {
			const auto shade = (uint8_t) round(255.0f * (float) (index + 1) / 16.0f);

			_palette[240 + index] = Rgb888Color(shade, shade, shade).toRgb565().getValue();
		}
	}
}