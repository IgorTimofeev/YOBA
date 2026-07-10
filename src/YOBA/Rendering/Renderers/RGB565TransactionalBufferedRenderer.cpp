#include <string.h>

#include <YOBA/Rendering/Renderers/RGB565TransactionalBufferedRenderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	size_t RGB565TransactionalBufferedRenderer::computePixelBufferLength() const {
		return _target->getSize().getSquare() * 2;
	}

	void RGB565TransactionalBufferedRenderer::flush() {
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

	void RGB565TransactionalBufferedRenderer::clearNative(const Color* color) {
		std::fill_n(
			reinterpret_cast<uint16_t*>(_pixelBuffer),
			getPixelBufferLength() / 2,
			static_cast<const RGB565Color*>(color)->getValue()
		);
	}

	void RGB565TransactionalBufferedRenderer::putPixelNative(const Point& point, const Color* color) {
		*(reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point))
			= static_cast<const RGB565Color*>(color)->getValue();
	}

	void RGB565TransactionalBufferedRenderer::strokeHorizontalLineNative(const Point& point, const uint16_t length, const Color* color) {
		std::fill_n(
			reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point),
			length,
			static_cast<const RGB565Color*>(color)->getValue()
		);
	}

	void RGB565TransactionalBufferedRenderer::strokeVerticalLineNative(const Point& point, const uint16_t length, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(point);
		const uint16_t scanlineLength = _target->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < length; i++) {
			*pixelBufferPtr = value;
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565TransactionalBufferedRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		auto pixelBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer) + getPixelIndex(bounds.getX(), bounds.getY());
		const uint16_t scanlineLength = _target->getSize().getWidth();
		const auto value = static_cast<const RGB565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill_n(pixelBufferPtr, bounds.getWidth(), value);
			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB565TransactionalBufferedRenderer::putImageNative(const Point& point, const Image* image) {
		if (image->getColorModel() != ColorModel::RGB565)
			return;

		const auto& clip = getClip();

		// With alpha
		if (image->getOptions() & ImageOptions::alpha1Bit) {
			auto bitmapPtr = image->getBitmap();

			uint8_t bitmapBitIndex = 0;

			// 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
			// ---- ---- | ---- -+-- | ---- ---- | ---- -2--
			for (uint16_t imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
				for (uint16_t imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
					const auto contains = clip.contains(Point(point.getX() + imageX, point.getY() + imageY));

					// Non-transparent
					if (*bitmapPtr & (1 << bitmapBitIndex)) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;

							if (contains) {
								const auto pixelBufferPtr =
									reinterpret_cast<uint16_t*>(_pixelBuffer)
									+ getPixelIndex(point);

								std::memcpy(pixelBufferPtr, bitmapPtr, 2);
							}
						}
						// Dark souls
						else {
							if (contains) {
								const auto pixelBufferPtr =
									reinterpret_cast<uint16_t*>(_pixelBuffer)
									+ getPixelIndex(point);

								*pixelBufferPtr = static_cast<uint16_t>((*reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex) & 0xFFFF);
							}
						}

						bitmapPtr += 2;
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;
						}
					}
				}
			}
		}
		// Without
		else {
			auto bitmapPtr = reinterpret_cast<const uint16_t*>(image->getBitmap());

			for (uint16_t imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
				for (uint16_t imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
					if (clip.contains(Point(point.getX() + imageX, point.getY() + imageY))) {
						const auto pixelBufferPtr =
							reinterpret_cast<uint16_t*>(_pixelBuffer)
							+ getPixelIndex(point);

						std::memcpy(pixelBufferPtr, bitmapPtr, 2);
					}

					bitmapPtr++;
				}
			}
		}
	}
}