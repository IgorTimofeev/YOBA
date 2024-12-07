#include "bit8PaletteBuffer.h"
#include "bounds.h"
#include "../drivers/transactionalSPIScreenDriver.h"

namespace yoba {
	Bit8PaletteBuffer::Bit8PaletteBuffer(ScreenDriver* driver, uint16_t paletteLength) : PaletteBuffer(driver, paletteLength) {

	}

	size_t Bit8PaletteBuffer::getRequiredBufferLength() {
		return getSize().getWidth() * getSize().getHeight();
	}

	void Bit8PaletteBuffer::flush() {
		switch (_driver->getPixelWritingType()) {
			case ScreenDriverPixelWritingType::Transactional: {
				const auto transactionalDriver = (TransactionalSPIScreenDriver*) _driver;

				switch (_driver->getColorModel()) {
					case ColorModel::Rgb565:
						transactionalDriver->writePixels([&](uint8_t*& destination, size_t& pixelIndex) {
							((uint16_t*) destination)[0] = ((uint16_t*) _palette)[_buffer[pixelIndex]];
							destination += 2;
							pixelIndex++;
						});

						break;

					case ColorModel::Rgb666:
						const uint8_t* palettePtr;

						transactionalDriver->writePixels([&](uint8_t*& destination, size_t& pixelIndex) {
							palettePtr = _palette + _buffer[pixelIndex] * 3;

							destination[0] = palettePtr[0];
							destination[1] = palettePtr[1];
							destination[2] = palettePtr[2];

							destination += 3;
							pixelIndex++;
						});

						break;

					default:
						break;
				}

				break;
			}
			default:
				break;
		}
	}

	void Bit8PaletteBuffer::clearNative(const Color* color) {
		memset(_buffer, (int) getPaletteIndex(color), _bufferLength);
	}

	void Bit8PaletteBuffer::renderPixelNative(const Point& point, const Color* color) {
		_buffer[getIndex(point)] = getPaletteIndex(color);
	}

	void Bit8PaletteBuffer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(_buffer + getIndex(point), getPaletteIndex(color), width);
	}

	void Bit8PaletteBuffer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(point);
		uint16_t scanlineLength = getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteBuffer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteBuffer::renderImageNative(const Point& point, const Image* image) {
		size_t
			bufferIndex = getIndex(point),
			scanlineLength = getSize().getWidth() - image->getSize().getWidth(),
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

	void Bit8PaletteBuffer::setOpenComputersPaletteColors() {
//		const uint8_t reds = 6;
//		const uint8_t greens = 8;
//		const uint8_t blues = 5;
//
//		for (uint8_t index = 0; index < 240; index++) {
//			const uint8_t idxB = index % blues;
//			const uint8_t idxG = (index / blues) % greens;
//			const uint8_t idxR = (index / blues / greens) % reds;
//
//			const auto r = (uint8_t) round((float) idxR * 255.0f / ((float) reds - 1.0f));
//			const auto g = (uint8_t) round((float) idxG * 255.0f / ((float) greens - 1.0f));
//			const auto b = (uint8_t) round((float) idxB * 255.0f / ((float) blues - 1.0f));
//
//			setPaletteColor(index, Rgb888Color(r, g, b));
//		}
//
//		for (uint8_t index = 0; index < 16; index++) {
//			const auto shade = (uint8_t) round(255.0f * (float) (index + 1) / 16.0f);
//
//			setPaletteColor(240 + index, Rgb888Color(shade, shade, shade));
//		}
	}
}