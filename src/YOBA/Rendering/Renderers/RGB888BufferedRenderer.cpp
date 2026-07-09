#include <YOBA/Rendering/Renderers/RGB888BufferedRenderer.hpp>

namespace YOBA {
	void RGB888BufferedRenderer::flush() {
		_target->flush(
			Rectangle(getTarget()->getSize()),
			{ _pixelBuffer, _pixelBufferLength }
		);
	}

	size_t RGB888BufferedRenderer::computePixelBufferLength() const {
		return getTarget()->getSize().getSquare() * 3;
	}

	void RGB888BufferedRenderer::clearNative(const Color* color) {
		const auto color8 = reinterpret_cast<const RGB888Color*>(color);

		// 1st pixel
		_pixelBuffer[0] = color8->getR();
		_pixelBuffer[1] = color8->getG();
		_pixelBuffer[2] = color8->getB();

		// Copying 1st pixel up to end of pixel buffer
		size_t copiedBytes = 3;

		while (copiedBytes * 2 <= _pixelBufferLength) {
			std::memcpy(_pixelBuffer + copiedBytes, _pixelBuffer, copiedBytes);
			copiedBytes *= 2;
		}

		// Copying remaining part
		if (copiedBytes < _pixelBufferLength)
			std::memcpy(_pixelBuffer + copiedBytes, _pixelBuffer, _pixelBufferLength - copiedBytes);
	}

	void RGB888BufferedRenderer::putPixelNative(const Point& point, const Color* color) {
		const auto pixelBufferPtr = _pixelBuffer + getPixelIndex(point) * 3;
		const auto color8 = reinterpret_cast<const RGB888Color*>(color);

		pixelBufferPtr[0] = color8->getR();
		pixelBufferPtr[1] = color8->getG();
		pixelBufferPtr[2] = color8->getB();
	}

	void RGB888BufferedRenderer::strokeHorizontalLineNative(const Point& point, const uint16_t length,
		const Color* color) {
		const auto pixel0Ptr = _pixelBuffer + getPixelIndex(point.getX(), point.getY()) * 3;
		const auto color8 = reinterpret_cast<const RGB888Color*>(color);

		// 1st pixel
		pixel0Ptr[0] = color8->getR();
		pixel0Ptr[1] = color8->getG();
		pixel0Ptr[2] = color8->getB();

		// Copying 1st pixel up to end of 1st row
		const size_t lengthBytes = length * 3;

		size_t copiedBytes = 3;

		while (copiedBytes * 2 <= lengthBytes) {
			std::memcpy(pixel0Ptr + copiedBytes, pixel0Ptr, copiedBytes);
			copiedBytes *= 2;
		}

		// Copying remaining part
		if (copiedBytes < lengthBytes)
			std::memcpy(pixel0Ptr + copiedBytes, pixel0Ptr, lengthBytes - copiedBytes);
	}

	void RGB888BufferedRenderer::
	strokeVerticalLineNative(const Point& point, const uint16_t length, const Color* color) {
		auto pixelBufferPtr = _pixelBuffer + getPixelIndex(point) * 3;
		const uint16_t scanlineLength = (_target->getSize().getWidth() - 1) * 3;
		const auto color8 = reinterpret_cast<const RGB888Color*>(color);

		for (uint16_t i = 0; i < length; i++) {
			*pixelBufferPtr = color8->getR();
			pixelBufferPtr++;

			*pixelBufferPtr = color8->getG();
			pixelBufferPtr++;

			*pixelBufferPtr = color8->getB();
			pixelBufferPtr++;

			pixelBufferPtr += scanlineLength;
		}
	}

	void RGB888BufferedRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		const auto pixel0Ptr = _pixelBuffer + getPixelIndex(bounds.getX(), bounds.getY()) * 3;
		const auto color8 = reinterpret_cast<const RGB888Color*>(color);

		// 1st pixel
		pixel0Ptr[0] = color8->getR();
		pixel0Ptr[1] = color8->getG();
		pixel0Ptr[2] = color8->getB();

		// Copying 1st pixel up to end of 1st row
		const size_t boundsWidthBytes = bounds.getWidth() * 3;

		size_t copiedBytes = 3;

		while (copiedBytes * 2 <= boundsWidthBytes) {
			std::memcpy(pixel0Ptr + copiedBytes, pixel0Ptr, copiedBytes);
			copiedBytes *= 2;
		}

		// Copying remaining part of 1st row
		if (copiedBytes < boundsWidthBytes)
			std::memcpy(pixel0Ptr + copiedBytes, pixel0Ptr, boundsWidthBytes - copiedBytes);

		// Copying 1st row down to end of rectangle
		const size_t scanlineWidth = getTarget()->getSize().getWidth() * 3;

		for (size_t y = 1; y < bounds.getHeight(); ++y)
			std::memcpy(pixel0Ptr + y * scanlineWidth, pixel0Ptr, boundsWidthBytes);
	}

	void RGB888BufferedRenderer::putImageNative(const Point& point, const Image* image) {
		if (image->getColorModel() != ColorModel::RGB888)
			return;

		const auto& clip = getClip();

		auto pixelBufferPtr = _pixelBuffer + getPixelIndex(point) * 3;
		const size_t pixelBufferScanlineLength = (_target->getSize().getWidth() - image->getSize().getWidth()) * 3;

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
								for (size_t i = 0; i < 3; ++i) {
									*pixelBufferPtr = *bitmapPtr;

									pixelBufferPtr++;
									bitmapPtr++;
								}
							}
							else {
								pixelBufferPtr += 3;
								bitmapPtr += 3;
							}
						}
						// Dark souls
						else {
							if (contains) {
								const auto value24Bit = (*reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex) & 0xFFFFFF;

								pixelBufferPtr[0] = value24Bit & 0xFF;
								pixelBufferPtr[1] = (value24Bit >> 8) & 0xFF;
								pixelBufferPtr[2] = (value24Bit >> 16) & 0xFF;
							}

							pixelBufferPtr += 3;
							bitmapPtr += 3;
						}
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;
						}

						pixelBufferPtr += 3;
					}
				}

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
		// Without
		else {
			auto bitmapPtr = image->getBitmap();

			for (uint16_t imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
				for (uint16_t imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
					if (clip.contains(Point(point.getX() + imageX, point.getY() + imageY))) {
						for (size_t i = 0; i < 3; ++i) {
							*pixelBufferPtr = *bitmapPtr;

							pixelBufferPtr++;
							bitmapPtr++;
						}
					}
					else {
						pixelBufferPtr += 3;
						bitmapPtr += 3;
					}
				}

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
	}
}