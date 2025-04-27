#include "RGB565PixelBufferRenderer.h"
#include "YOBA/main/bounds.h"
#include <string.h>

namespace YOBA {
	size_t RGB565PixelBufferRenderer::computePixelBufferLength() const {
		return getTarget()->getSize().getSquare() * 2;
	}

	size_t RGB565PixelBufferRenderer::getPixelBufferIndex(uint16_t x, uint16_t y) const {
		return (y * getTarget()->getSize().getWidth() + x) * 2;
	}

	void RGB565PixelBufferRenderer::flush() {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565: {
				auto pixelBufferPtr = getPixelBuffer();
				const auto& size = getTarget()->getSize();
				const auto transactionWindowHeight = computeTransactionWindowHeight();
				const size_t transactionLength = size.getWidth() * transactionWindowHeight * 2;

				for (uint16_t y = 0; y < size.getHeight(); y += transactionWindowHeight) {
					getTarget()->writePixels(
						Bounds(0, y, size.getWidth(), transactionWindowHeight),
						pixelBufferPtr,
						transactionLength
					);

					pixelBufferPtr += transactionLength;
				}

				break;
			}
			default:
				break;
		}
	}

	void RGB565PixelBufferRenderer::updateFromTarget() {
		PixelBufferRenderer::updateFromTarget();
	}

	void RGB565PixelBufferRenderer::clearNative(const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer());

		std::fill(pixelBufferPtr, pixelBufferPtr + getPixelBufferLength() / 2, static_cast<const Rgb565Color*>(color)->getValue());
	}

	void RGB565PixelBufferRenderer::renderPixelNative(const Point& point, const Color* color) {
		*reinterpret_cast<uint16_t*>(getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point)) = static_cast<const Rgb565Color*>(color)->getValue();
	}

	void RGB565PixelBufferRenderer::renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point));

		std::fill(pixelBufferPtr, pixelBufferPtr + length, static_cast<const Rgb565Color*>(color)->getValue());
	}

	void RGB565PixelBufferRenderer::renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point));
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const Rgb565Color*>(color)->getValue();

		for (size_t i = 0; i < length; i++) {
			std::fill(pixelBufferPtr, pixelBufferPtr + 1, value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer() + getPixelBufferIndex(bounds.getX(), bounds.getY()));
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const Rgb565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill(pixelBufferPtr, pixelBufferPtr + bounds.getWidth(), value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderImageNative(const Point& point, const Image* image) {
		if (!(image->getFlags() & ImageFlags::RGB565))
			return;

		// With alpha
		if (image->getFlags() & ImageFlags::alpha1Bit) {
			auto pixelBufferPtr = getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point);
			auto bitmapPtr = image->getBitmap();
			const size_t scanlineLength = (getTarget()->getSize().getWidth() - image->getSize().getWidth()) * 2;

			uint8_t bitmapBitIndex = 0;

			// 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
			// ---- ---- | ---- -+-- | ---- ---- | ---- -2--
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					// Non-transparent
					if ((*bitmapPtr) & (1 << bitmapBitIndex)) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;

							// 1
							*pixelBufferPtr = *bitmapPtr;
							pixelBufferPtr++;
							bitmapPtr++;

							// 2
							*pixelBufferPtr = *bitmapPtr;
							pixelBufferPtr++;
							bitmapPtr++;
						}
						// Dark souls
						else {
							*reinterpret_cast<uint16_t*>(pixelBufferPtr) = static_cast<uint16_t>((*reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex) & 0xFFFF);

							bitmapPtr += 2;
							pixelBufferPtr += 2;
						}
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr += 1;
						}

						pixelBufferPtr += 2;
					}
				}

				pixelBufferPtr += scanlineLength;
			}
		}
		// Without
		else {
			auto pixelBufferPtr = reinterpret_cast<uint16_t*>(getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point));
			auto bitmapPtr = reinterpret_cast<const uint16_t*>(image->getBitmap());
			const size_t scanlineLength = getTarget()->getSize().getWidth() - image->getSize().getWidth();

			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					*pixelBufferPtr = *bitmapPtr;

					pixelBufferPtr++;
					bitmapPtr++;
				}

				pixelBufferPtr += scanlineLength;
			}
		}
	}
}