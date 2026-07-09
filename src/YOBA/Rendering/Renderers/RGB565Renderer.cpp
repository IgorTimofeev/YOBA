#include <string.h>

#include <YOBA/Rendering/Renderers/RGB565Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	size_t RGB565Renderer::computePixelBufferLength() const {
		return _target->getSize().getSquare() * 2;
	}

	void RGB565Renderer::flush() {
		const auto& size = _target->getSize();
		auto transactionBufferPtr = _pixelBuffer;
		const size_t transactionBufferLength = size.getWidth() * _transactionViewportHeight * 2;

		for (uint16_t y = 0; y < size.getHeight(); y += _transactionViewportHeight) {
			_target->flush(
				Rectangle(0, y, size.getWidth(), getTransactionViewportHeight()),
				{ transactionBufferPtr, transactionBufferLength }
			);

			transactionBufferPtr += transactionBufferLength;
		}
	}

	void RGB565Renderer::clearNative(const Color* color) {
		std::fill_n(
			reinterpret_cast<uint16_t*>(_pixelBuffer),
			getPixelBufferLength() / 2,
			static_cast<const RGB565Color*>(color)->getValue()
		);
	}

	void RGB565Renderer::renderPixelNative(const Point& point, const Color* color) {
		*(reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point))
			= static_cast<const RGB565Color*>(color)->getValue();
	}

	void RGB565Renderer::renderHorizontalLineNative(const Point& point, const uint16_t length, const Color* color) {
		std::fill_n(
			reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point),
			length,
			static_cast<const RGB565Color*>(color)->getValue()
		);
	}

	void RGB565Renderer::renderVerticalLineNative(const Point& point, const uint16_t length, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);
		const uint16_t scanlineLength = _target->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < length; i++) {
			*pixelBufferPtr = value;
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565Renderer::renderFilledRectangleNative(const Rectangle& bounds, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(bounds.getX(), bounds.getY());
		const uint16_t scanlineLength = _target->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill_n(pixelBufferPtr, bounds.getWidth(), value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565Renderer::renderImageNative(const Point& point, const Image* image) {
		if (!(image->getOptions() & ImageOptions::RGB565))
			return;

		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);
		const size_t pixelBufferScanlineLength = _target->getSize().getWidth() - image->getSize().getWidth();

		// With alpha
		if (image->getOptions() & ImageOptions::alpha1Bit) {
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