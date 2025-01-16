#include "monochromeBufferedRenderer.h"
#include "hardware/displays/directWritingDisplay.h"

namespace yoba {
	using namespace yoba::hardware;

	MonochromeBufferedRenderer::MonochromeBufferedRenderer(RenderTarget* renderTarget) : BufferedRenderer(renderTarget) {

	}

	size_t MonochromeBufferedRenderer::getRequiredBufferLength() {
		return getRenderTarget()->getResolution().getSquare() / 8;
	}

	void MonochromeBufferedRenderer::flush() {
		switch (getRenderTarget()->getPixelWriting()) {
			case RenderTargetPixelWriting::direct: {
				(dynamic_cast<DirectWritingDisplay*>(getRenderTarget()))->writePixels(getBuffer());

				break;
			}
			default:
				break;
		}
	}

	void MonochromeBufferedRenderer::clearNative(const Color* color) {
		memset(getBuffer(), ((MonochromeColor*) color)->getValue() ? 0xFF : 0x00, getBufferLength());
	}

	void MonochromeBufferedRenderer::renderPixelNative(const Point& point, const Color* color) {
		switch (getRenderTarget()->getPixelOrder()) {
			case RenderTargetPixelOrder::XY: {
				if (((MonochromeColor*) color)->getValue()) {
					getBuffer()[point.getX() + (point.getY() / 8) * getRenderTarget()->getResolution().getWidth()] |= (1 << (point.getY() & 7));
				}
				else {
					getBuffer()[point.getX() + (point.getY() / 8) * getRenderTarget()->getResolution().getWidth()] &= ~(1 << (point.getY() & 7));
				}

				break;
			}
			case RenderTargetPixelOrder::XYReversed: {
				if (((MonochromeColor*) color)->getValue()) {
					getBuffer()[point.getX() + (point.getY() / 8) * getRenderTarget()->getResolution().getWidth()] |= (1 << (7 - (point.getY() % 8)));
				}
				else {
					getBuffer()[point.getX() + (point.getY() / 8) * getRenderTarget()->getResolution().getWidth()] &= ~(1 << (7 - (point.getY() % 8)));
				}

				break;
			}
			default:
				break;
		}
	}

	void MonochromeBufferedRenderer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		for (int32_t x = point.getX(); x < point.getX() + width; x++) {
			renderPixelNative(Point(x, point.getY()), color);
		}
	}

	void MonochromeBufferedRenderer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		for (int32_t y = point.getY(); y < point.getY() + height; y++) {
			renderPixelNative(Point(point.getX(), y), color);
		}
	}

	void MonochromeBufferedRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		for (int32_t y = bounds.getY(); y < bounds.getY() + bounds.getHeight(); y++) {
			for (int32_t x = bounds.getX(); x < bounds.getX() + bounds.getWidth(); x++) {
				renderPixelNative(Point(x, y), color);
			}
		}
	}

	void MonochromeBufferedRenderer::renderImageNative(const Point& point, const Image* image) {

	}

	void MonochromeBufferedRenderer::printBufferContentsAsBinary() {
		Serial.printf("Monochrome screen buffer %d x %d:\n", getRenderTarget()->getResolution().getWidth(), getRenderTarget()->getResolution().getHeight());

		size_t bufferPtr = 0;

		for (int j = 0; j < getRenderTarget()->getResolution().getHeight(); j++) {
			for (int i = 0; i < getRenderTarget()->getResolution().getWidth(); i += 8) {
				for (int k = 0; k < 8; k++) {
					Serial.print((((getBuffer()[bufferPtr] >> (7 - k)) & 0b1) == 1) ? "#" : ".");
				}

				bufferPtr++;
			}

			Serial.println();
		}
	}
}