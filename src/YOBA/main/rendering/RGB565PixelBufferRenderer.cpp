#include "RGB565PixelBufferRenderer.h"
#include "YOBA/main/bounds.h"
#include <string.h>

namespace YOBA {
	size_t RGB565PixelBufferRenderer::computePixelBufferLength() const {
		return getTarget()->getSize().getSquare() * 2;
	}

	void RGB565PixelBufferRenderer::flush() {
		switch (getTarget()->getColorModel()) {
			case ColorModel::RGB565: {
				getTarget()->writePixels(
					Bounds(getTarget()->getSize()),
					getPixelBuffer(),
					getPixelBufferLength()
				);

				break;
			}
			default:
				break;
		}
	}

	void RGB565PixelBufferRenderer::clearNative(const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer());

		std::fill(pixelBufferPtr, pixelBufferPtr + getPixelBufferLength() / 2, static_cast<const RGB565Color*>(color)->getValue());
	}

	void RGB565PixelBufferRenderer::renderPixelNative(const Point& point, const Color* color) {
		*(reinterpret_cast<uint16_t*>(getPixelBuffer()) + getPixelIndex(point)) = static_cast<const RGB565Color*>(color)->getValue();
	}

	void RGB565PixelBufferRenderer::renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer()) + getPixelIndex(point);

		std::fill(pixelBufferPtr, pixelBufferPtr + length, static_cast<const RGB565Color*>(color)->getValue());
	}

	void RGB565PixelBufferRenderer::renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer()) + getPixelIndex(point);
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < length; i++) {
			*pixelBufferPtr = value;
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer()) + getPixelIndex(bounds.getX(), bounds.getY());
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill(pixelBufferPtr, pixelBufferPtr + bounds.getWidth(), value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderImageNative(const Point& point, const Image* image) {
		if (!(image->getFlags() & ImageFlags::RGB565))
			return;

		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer()) + getPixelIndex(point);
		const size_t pixelBufferScanlineLength = getTarget()->getSize().getWidth() - image->getSize().getWidth();

		// With alpha
		if (image->getFlags() & ImageFlags::alpha1Bit) {
			auto bitmapPtr = image->getBitmap();

			uint8_t bitmapBitIndex = 0;

			// 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
			// ---- ---- | ---- -+-- | ---- ---- | ---- -2--
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					// Non-transparent
					if (*bitmapPtr & (1 << bitmapBitIndex)) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;

							*pixelBufferPtr = *reinterpret_cast<const uint16_t*>(bitmapPtr);
							pixelBufferPtr++;
							bitmapPtr += 2;
						}
						// Dark souls
						else {
							*pixelBufferPtr = static_cast<uint16_t>((*reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex) & 0xFFFF);

							pixelBufferPtr++;
							bitmapPtr += 2;
						}
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;
						}

						pixelBufferPtr++;
					}
				}

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
		// Without
		else {
			auto bitmapPtr = reinterpret_cast<const uint16_t*>(image->getBitmap());

			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					*pixelBufferPtr = *bitmapPtr;

					pixelBufferPtr++;
					bitmapPtr++;
				}

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
	}
}