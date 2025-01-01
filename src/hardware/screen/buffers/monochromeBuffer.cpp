#include "monochromeBuffer.h"
#include "../drivers/directScreenDriver.h"

namespace yoba {
	MonochromeBuffer::MonochromeBuffer(ScreenDriver* driver) : ScreenBuffer(driver) {
		setDefaultPrimaryColor(&_defaultPrimaryColor);
		setDefaultSecondaryColor(&_defaultSecondaryColor);
	}

	size_t MonochromeBuffer::getRequiredBufferLength() {
		return getSize().getSquare() / 8;
	}

	void MonochromeBuffer::flush() {
		switch (_driver->getPixelWritingMethod()) {
			case ScreenPixelWritingMethod::Direct: {
				(dynamic_cast<DirectScreenDriver*>(_driver))->writePixels(_buffer);

				break;
			}
			default:
				break;
		}
	}

	void MonochromeBuffer::clearNative(const Color* color) {
		memset(_buffer, ((MonochromeColor*) color)->getValue() ? 0xFF : 0x00, _bufferLength);
	}

	void MonochromeBuffer::renderPixelNative(const Point& point, const Color* color) {
		switch (_driver->getPixelAlignment()) {
			case ScreenPixelAlignment::XNormalYNormal: {
				if (((MonochromeColor*) color)->getValue()) {
					_buffer[point.getX() + (point.getY() / 8) * getSize().getWidth()] |= (1 << (point.getY() & 7));
				}
				else {
					_buffer[point.getX() + (point.getY() / 8) * getSize().getWidth()] &= ~(1 << (point.getY() & 7));
				}

				break;
			}
			case ScreenPixelAlignment::XNormalYReversed: {
				if (((MonochromeColor*) color)->getValue()) {
					_buffer[point.getX() + (point.getY() / 8) * getSize().getWidth()] |= (1 << (7 - (point.getY() % 8)));
				}
				else {
					_buffer[point.getX() + (point.getY() / 8) * getSize().getWidth()] &= ~(1 << (7 - (point.getY() % 8)));
				}

				break;
			}
			default:
				break;
		}
	}

	void MonochromeBuffer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		for (int32_t x = point.getX(); x < point.getX() + width; x++) {
			renderPixelNative(Point(x, point.getY()), color);
		}
	}

	void MonochromeBuffer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		for (int32_t y = point.getY(); y < point.getY() + height; y++) {
			renderPixelNative(Point(point.getX(), y), color);
		}
	}

	void MonochromeBuffer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		for (int32_t y = bounds.getY(); y < bounds.getY() + bounds.getHeight(); y++) {
			for (int32_t x = bounds.getX(); x < bounds.getX() + bounds.getWidth(); x++) {
				renderPixelNative(Point(x, y), color);
			}
		}
	}

	void MonochromeBuffer::renderImageNative(const Point& point, const Image* image) {

	}

	void MonochromeBuffer::printBufferContentsAsBinary() {
		Serial.printf("Monochrome screen buffer %d x %d:\n", getSize().getWidth(), getSize().getHeight());

		size_t bufferPtr = 0;

		for (int j = 0; j < getSize().getHeight(); j++) {
			for (int i = 0; i < getSize().getWidth(); i += 8) {
				for (int k = 0; k < 8; k++) {
					Serial.print((((_buffer[bufferPtr] >> (7 - k)) & 0b1) == 1) ? "#" : ".");
				}

				bufferPtr++;
			}

			Serial.println();
		}
	}
}