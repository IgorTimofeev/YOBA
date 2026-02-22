#include "monochromeRenderer.h"

namespace YOBA {
	size_t MonochromeRenderer::computePixelBufferLength() const {
		return getTarget().getSize().getSquare() / 8;
	}

	void MonochromeRenderer::flush() {
		getTarget().writePixels(Bounds(getTarget().getSize()), getPixelBuffer(), getPixelBufferLength());
	}

	void MonochromeRenderer::clearNative(const Color& color) {
		memset(getPixelBuffer(), reinterpret_cast<const MonochromeColor&>(color).getValue() ? 0xFF : 0x00, getPixelBufferLength());
	}

	void MonochromeRenderer::renderPixelNative(const Point& point, const Color& color) {
		switch (getTarget().getPixelOrder()) {
			case PixelOrder::XY: {
				if (reinterpret_cast<const MonochromeColor&>(color).getValue()) {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget().getSize().getWidth()] |= (1 << (point.getY() & 7));
				}
				else {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget().getSize().getWidth()] &= ~(1 << (point.getY() & 7));
				}

				break;
			}
			case PixelOrder::XYReversed: {
				if (reinterpret_cast<const MonochromeColor&>(color).getValue()) {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget().getSize().getWidth()] |= (1 << (7 - (point.getY() % 8)));
				}
				else {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget().getSize().getWidth()] &= ~(1 << (7 - (point.getY() % 8)));
				}

				break;
			}
			default:
				break;
		}
	}

	void MonochromeRenderer::renderHorizontalLineNative(const Point& point, const uint16_t width, const Color& color) {
		for (int32_t x = point.getX(); x < point.getX() + width; x++) {
			renderPixelNative(Point(x, point.getY()), color);
		}
	}

	void MonochromeRenderer::renderVerticalLineNative(const Point& point, const uint16_t height, const Color& color) {
		for (int32_t y = point.getY(); y < point.getY() + height; y++) {
			renderPixelNative(Point(point.getX(), y), color);
		}
	}

	void MonochromeRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color& color) {
		for (int32_t y = bounds.getY(); y < bounds.getY() + bounds.getHeight(); y++) {
			for (int32_t x = bounds.getX(); x < bounds.getX() + bounds.getWidth(); x++) {
				renderPixelNative(Point(x, y), color);
			}
		}
	}

	void MonochromeRenderer::renderImageNative(const Point& point, const Image& image) {

	}
}