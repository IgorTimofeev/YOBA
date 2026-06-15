#include "RGB565Renderer.h"
#include <YOBA/core/rectangle.h>
#include <string.h>

namespace YOBA {
	size_t RGB565Renderer::computePixelBufferLength() const {
		return getTarget()->getSize().getSquare() * 2;
	}

	void RGB565Renderer::flush() {
		const auto& size = getTarget()->getSize();
		const size_t transactionLength = size.getWidth() * _transactionViewportHeight * 2;
		auto transactionBufferPtr = _pixelBuffer;

		for (uint16_t y = 0; y < size.getHeight(); y += _transactionViewportHeight) {
			getTarget()->writePixels(
				Rectangle(0, y, size.getWidth(), getTransactionViewportHeight()),
				{ transactionBufferPtr, transactionLength }
			);

			transactionBufferPtr += transactionLength;
		}
	}

	void RGB565Renderer::clearNative(const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer);

		std::fill_n(pixelBufferPtr, getPixelBufferLength() / 2, static_cast<const RGB565Color*>(color)->getValue());
	}

	void RGB565Renderer::renderPixelNative(const Point& point, const Color* color) {
		*(reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point)) = static_cast<const RGB565Color*>(color)->getValue();
	}

	void RGB565Renderer::renderHorizontalLineNative(const Point& point, const uint16_t length, const Color* color) {
		const auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);

		std::fill_n(pixelBufferPtr, length, static_cast<const RGB565Color*>(color)->getValue());
	}

	void RGB565Renderer::renderVerticalLineNative(const Point& point, const uint16_t length, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < length; i++) {
			*pixelBufferPtr = value;
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565Renderer::renderFilledRectangleNative(const Rectangle& bounds, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(bounds.getX(), bounds.getY());
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill_n(pixelBufferPtr, bounds.getWidth(), value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565Renderer::renderImageNative(const Point& point, const Image* image) {
		if (!(image->getFlags() & ImageFlags::RGB565))
			return;

		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);
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