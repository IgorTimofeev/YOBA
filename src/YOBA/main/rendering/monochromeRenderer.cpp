#include "monochromeRenderer.h"
#include "directRenderTarget.h"

namespace YOBA {
	size_t MonochromeRenderer::getRequiredBufferLength() {
		return getTarget()->getSize().getSquare() / 8;
	}

	void MonochromeRenderer::flushBuffer() {
		switch (getTarget()->getPixelWriting()) {
			case PixelWriting::direct: {
				(dynamic_cast<DirectRenderTarget*>(getTarget()))->writePixels(getBuffer());

				break;
			}
			default:
				break;
		}
	}

	void MonochromeRenderer::clearNative(const Color* color) {
		memset(getBuffer(), ((MonochromeColor*) color)->getValue() ? 0xFF : 0x00, getBufferLength());
	}

	void MonochromeRenderer::renderPixelNative(const Point& point, const Color* color) {
		switch (getTarget()->getPixelOrder()) {
			case PixelOrder::XY: {
				if (((MonochromeColor*) color)->getValue()) {
					getBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] |= (1 << (point.getY() & 7));
				}
				else {
					getBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] &= ~(1 << (point.getY() & 7));
				}

				break;
			}
			case PixelOrder::XYReversed: {
				if (((MonochromeColor*) color)->getValue()) {
					getBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] |= (1 << (7 - (point.getY() % 8)));
				}
				else {
					getBuffer()[point.getX() + (point.getY() / 8) * getTarget()->getSize().getWidth()] &= ~(1 << (7 - (point.getY() % 8)));
				}

				break;
			}
			default:
				break;
		}
	}

	void MonochromeRenderer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		for (int32_t x = point.getX(); x < point.getX() + width; x++) {
			renderPixelNative(Point(x, point.getY()), color);
		}
	}

	void MonochromeRenderer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		for (int32_t y = point.getY(); y < point.getY() + height; y++) {
			renderPixelNative(Point(point.getX(), y), color);
		}
	}

	void MonochromeRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		for (int32_t y = bounds.getY(); y < bounds.getY() + bounds.getHeight(); y++) {
			for (int32_t x = bounds.getX(); x < bounds.getX() + bounds.getWidth(); x++) {
				renderPixelNative(Point(x, y), color);
			}
		}
	}

	void MonochromeRenderer::renderImageNative(const Point& point, const Image* image) {

	}
}