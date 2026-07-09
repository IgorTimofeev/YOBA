#include <YOBA/Rendering/Renderers/MonochromeBufferedRenderer.hpp>

namespace YOBA {
	size_t MonochromeBufferedRenderer::computePixelBufferLength() const {
		return getTarget()->getSize().getSquare() / 8;
	}

	void MonochromeBufferedRenderer::flush() {
		getTarget()->flush(
			Rectangle(getTarget()->getSize()),
			{ getPixelBuffer(), getPixelBufferLength() }
		);
	}

	void MonochromeBufferedRenderer::clearNative(const Color* color) {
		memset(getPixelBuffer(), reinterpret_cast<const MonochromeColor*>(color)->getValue() ? 0xFF : 0x00, getPixelBufferLength());
	}

	void MonochromeBufferedRenderer::putPixelNative(const Point& point, const Color* color) {
		switch (getTarget()->getPixelOrder()) {
			case PixelOrder::XNormalYNormal: {
				if (reinterpret_cast<const MonochromeColor*>(color)->getValue()) {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] |= (1 << (point.getY() & 7));
				}
				else {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] &= ~(1 << (point.getY() & 7));
				}

				break;
			}
			case PixelOrder::XNormalYReversed: {
				if (reinterpret_cast<const MonochromeColor*>(color)->getValue()) {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] |= (1 << (7 - (point.getY() % 8)));
				}
				else {
					getPixelBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] &= ~(1 << (7 - (point.getY() % 8)));
				}

				break;
			}
			default:
				break;
		}
	}

	void MonochromeBufferedRenderer::strokeHorizontalLineNative(const Point& point, const uint16_t width, const Color* color) {
		for (int32_t x = point.getX(); x < point.getX() + width; x++) {
			putPixelNative(Point(x, point.getY()), color);
		}
	}

	void MonochromeBufferedRenderer::strokeVerticalLineNative(const Point& point, const uint16_t height, const Color* color) {
		for (int32_t y = point.getY(); y < point.getY() + height; y++) {
			putPixelNative(Point(point.getX(), y), color);
		}
	}

	void MonochromeBufferedRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		for (int32_t y = bounds.getY(); y < bounds.getY() + bounds.getHeight(); y++) {
			for (int32_t x = bounds.getX(); x < bounds.getX() + bounds.getWidth(); x++) {
				putPixelNative(Point(x, y), color);
			}
		}
	}

	void MonochromeBufferedRenderer::putImageNative(const Point& point, const Image* image) {

	}
}