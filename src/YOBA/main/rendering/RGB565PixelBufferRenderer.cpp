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
		getPixelBuffer()[PixelBufferRenderer::getPixelBufferIndex(point)] = static_cast<const Rgb565Color*>(color)->getValue();
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

	}
}