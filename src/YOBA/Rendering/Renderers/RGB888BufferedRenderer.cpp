#include <cstring>

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

	void RGB888BufferedRenderer::putImageNative(const Point& position, const Image* image) {
		if (image->getColorModel() != ColorModel::RGB888)
			return;

		auto x = position.getX();
		auto y = position.getY();

		uint16_t imageY;
		uint16_t imageX;

		bool containsY;

		const auto& clip = getClip();
		const auto clipX1 = clip.getX();
		const auto clipY1 = clip.getY();
		const auto clipX2 = clip.getX2();
		const auto clipY2 = clip.getY2();

		auto pixelBufferPtr = _pixelBuffer + getPixelIndex(x, y) * 3;
		const auto pixelBufferScanlineLength = (getTarget()->getSize().getWidth() - image->getSize().getWidth()) * 3;

		auto bitmapPtr = image->getBitmap();

		// With alpha
		if (image->getOptions() & ImageOptions::alpha1Bit) {
			uint8_t bitmapBitIndex = 0;
			bool containsYX;
			uint32_t value24Bit;

			// 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
			// ---- ---- | ---- -+-- | ---- ---- | ---- -2--
			for (imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
				containsY = y >= clipY1 && y <= clipY2;

				for (imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
					containsYX = containsY && x >= clipX1 && x <= clipX2;

					// Non-transparent
					if (*bitmapPtr & (1 << bitmapBitIndex)) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;

							if (containsYX)
								std::memcpy(pixelBufferPtr, bitmapPtr, 3);
						}
						// Dark souls
						else {
							if (containsYX) {
								value24Bit = *reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex;

								std::memcpy(pixelBufferPtr, &value24Bit, 3);
							}
						}

						bitmapPtr += 3;
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;
						}
					}

					x++;

					pixelBufferPtr += 3;
				}

				x = position.getX();
				y++;

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
		// Without
		else {
			for (imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
				containsY = y >= clipY1 && y <= clipY2;

				for (imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
					if (containsY && x >= clipX1 && x <= clipX2)
						std::memcpy(pixelBufferPtr, bitmapPtr, 3);

					x++;

					pixelBufferPtr += 3;
					bitmapPtr += 3;
				}

				x = position.getX();
				y++;

				pixelBufferPtr += pixelBufferScanlineLength;
			}
		}
	}
}