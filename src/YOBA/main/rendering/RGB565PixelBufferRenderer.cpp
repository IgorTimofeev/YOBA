#include "RGB565PixelBufferRenderer.h"
#include "YOBA/main/bounds.h"
#include <string.h>

namespace YOBA {
	size_t RGB565PixelBufferRenderer::computePixelBufferLengthForTarget() {
		return getTarget()->getSize().getSquare() * 2;
	}

	size_t RGB565PixelBufferRenderer::getPixelBufferIndex(uint16_t x, uint16_t y) const {
		return (y * getTarget()->getSize().getWidth() + x) * 2;
	}

	void RGB565PixelBufferRenderer::flush() {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565: {
				auto bufferPtr = getPixelBuffer();

				const uint16_t transactionHeight = 80;
				size_t transactionLength = getTarget()->getSize().getWidth() * transactionHeight * 2;

				for (uint16_t y = 0; y < getTarget()->getSize().getHeight(); y += transactionHeight) {
					getTarget()->writePixels(
						Bounds(0, y, getTarget()->getSize().getWidth(), transactionHeight),
						bufferPtr,
						transactionLength
					);

					bufferPtr += transactionLength;
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
		const auto ptr = reinterpret_cast<uint16_t*>(getPixelBuffer());

		std::fill(ptr, ptr + getPixelBufferLength() / 2, static_cast<const Rgb565Color*>(color)->getValue());
	}

	void RGB565PixelBufferRenderer::renderPixelNative(const Point& point, const Color* color) {
		getPixelBuffer()[PixelBufferRenderer::getPixelBufferIndex(point)] = static_cast<const Rgb565Color*>(color)->getValue();
	}

	void RGB565PixelBufferRenderer::renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) {
		memset(getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point), static_cast<const Rgb565Color*>(color)->getValue(), length);
	}

	void RGB565PixelBufferRenderer::renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) {
		uint8_t* bufferPtr = getPixelBuffer() + PixelBufferRenderer::getPixelBufferIndex(point);
		uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const Rgb565Color*>(color)->getValue();

		for (size_t i = 0; i < length; i++) {
			memset(bufferPtr, value, 1);
			bufferPtr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		auto ptr = reinterpret_cast<uint16_t*>(getPixelBuffer() + getPixelBufferIndex(bounds.getX(), bounds.getY()));
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto value = static_cast<const Rgb565Color*>(color)->getValue();

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::fill(ptr, ptr + bounds.getWidth(), value);
			ptr += scanlineLength;
		}
	}

	void RGB565PixelBufferRenderer::renderImageNative(const Point& point, const Image* image) {

	}
}