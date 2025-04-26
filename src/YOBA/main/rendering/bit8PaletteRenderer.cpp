#include "bit8PaletteRenderer.h"
#include "YOBA/main/bounds.h"

namespace YOBA {
	Bit8PaletteRenderer::Bit8PaletteRenderer(uint16_t paletteLength) : PaletteBufferRenderer(paletteLength) {

	}

	size_t Bit8PaletteRenderer::computePaletteBufferLength() {
		return getTarget()->getSize().getSquare();
	}

	void Bit8PaletteRenderer::flush(uint16_t width, const std::function<void(uint8_t*&, uint32_t&)>& pixelSetter) {
		uint32_t pixelIndex = 0;
		uint16_t y;
		uint8_t* transactionBufferStart;
		uint8_t* transactionBufferEnd = getTransactionBuffer() + getTransactionBufferLength();

		for (y = 0; y < getTarget()->getSize().getHeight(); y += getTransactionBufferHeight()) {
			transactionBufferStart = getTransactionBuffer();

			while (transactionBufferStart < transactionBufferEnd) {
				pixelSetter(transactionBufferStart, pixelIndex);
			}

			getTarget()->writePixels(
				Bounds(0, y, width, getTransactionBufferHeight()),
				getTransactionBuffer(),
				getTransactionBufferLength()
			);
		}
	}

	void Bit8PaletteRenderer::flush() {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565: {
				flush(
					getTarget()->getSize().getWidth(),
					[this](uint8_t*& transactionBuffer, uint32_t& pixelIndex) {
						*((uint16_t*) transactionBuffer) = ((uint16_t*) getPalette())[getPaletteBuffer()[pixelIndex]];
						transactionBuffer += 2;
						pixelIndex++;
					}
				);

				break;
			}

			case ColorModel::rgb666: {
				const uint8_t* palettePtr;

				flush(
					getTarget()->getSize().getWidth(),
					[&](uint8_t*& transactionBuffer, uint32_t& pixelIndex) {
						palettePtr = getPalette() + getPaletteBuffer()[pixelIndex] * 3;

						transactionBuffer[0] = palettePtr[0];
						transactionBuffer[1] = palettePtr[1];
						transactionBuffer[2] = palettePtr[2];

						transactionBuffer += 3;
						pixelIndex++;
					}
				);

				break;
			}
			default:
				break;
		}
	}

	void Bit8PaletteRenderer::clearNative(const Color* color) {
		memset(getPaletteBuffer(), (int) getPaletteIndex(color), getPaletteBufferLength());
	}

	void Bit8PaletteRenderer::renderPixelNative(const Point& point, const Color* color) {
//		if (point.getX() < 0 || point.getX() >= 320 || point.getY() < 0 || point.getY() >= 240) {
//			Serial.printf("CYKA?? %d, %d\f", point.getX(), point.getY());
//			return;
//		}
//
//		if (getIndex(point) >= getBufferLength()) {
//			Serial.printf("getIndex POINT %d, %d\f", point.getX(), point.getY());
//			Serial.printf("getIndex INDEX %d, %d\f", getIndex(point), getBufferLength());
//			return;
//		}

		getPaletteBuffer()[getPaletteBufferIndex(point)] = getPaletteIndex(color);
	}

	void Bit8PaletteRenderer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(getPaletteBuffer() + getPaletteBufferIndex(point), getPaletteIndex(color), width);
	}

	void Bit8PaletteRenderer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = getPaletteBuffer() + getPaletteBufferIndex(point);
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto paletteIndex = getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = getPaletteBuffer() + getPaletteBufferIndex(bounds.getPosition());
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteRenderer::renderImageNative(const Point& point, const Image* image) {
		if (!(image->getFlags() & ImageFlags::palette8Bit))
			return;

		size_t
			bufferIndex = getPaletteBufferIndex(point),
			scanlineLength = getTarget()->getSize().getWidth() - image->getSize().getWidth(),
			bitmapByteIndex = 0;

		uint8_t bitmapByte;

		// With alpha
		if (image->getFlags() & ImageFlags::alpha1Bit) {
			uint8_t bitmapBitIndex = 0;
			uint8_t part1;

			// 0000 0000
			// ---- -2--
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					bitmapByte = image->getBitmap()[bitmapByteIndex];

					// Non-transparent
					if (bitmapByte & (1 << bitmapBitIndex)) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapByteIndex++;

							getPaletteBuffer()[bufferIndex] = image->getBitmap()[bitmapByteIndex];

							bitmapByteIndex++;
						}
						// Dark souls
						else {
							part1 = bitmapByte >> bitmapBitIndex;
							bitmapByteIndex++;

							bitmapByte = image->getBitmap()[bitmapByteIndex];

							getPaletteBuffer()[bufferIndex] = part1 | (bitmapByte << (8 - bitmapBitIndex));
						}
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapByteIndex++;
						}
					}

					bufferIndex++;
				}

				bufferIndex += scanlineLength;
			}
		}
		// Without
		else {
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					getPaletteBuffer()[bufferIndex] = image->getBitmap()[bitmapByteIndex];

					bufferIndex++;
					bitmapByteIndex++;
				}

				bufferIndex += scanlineLength;
			}
		}
	}

	void Bit8PaletteRenderer::setOpenComputersPaletteColors() {
		const uint8_t reds = 6;
		const uint8_t greens = 8;
		const uint8_t blues = 5;

		for (uint8_t index = 0; index < 240; index++) {
			const uint8_t idxB = index % blues;
			const uint8_t idxG = (index / blues) % greens;
			const uint8_t idxR = (index / blues / greens) % reds;

			const auto r = (uint8_t) std::round((float) idxR * 255.0f / ((float) reds - 1.0f));
			const auto g = (uint8_t) std::round((float) idxG * 255.0f / ((float) greens - 1.0f));
			const auto b = (uint8_t) std::round((float) idxB * 255.0f / ((float) blues - 1.0f));

			setPaletteColor(index, Rgb888Color(r, g, b));
		}

		for (uint8_t index = 0; index < 16; index++) {
			const auto shade = (uint8_t) std::round(255.0f * (float) (index + 1) / 16.0f);

			setPaletteColor(240 + index, Rgb888Color(shade, shade, shade));
		}
	}

}