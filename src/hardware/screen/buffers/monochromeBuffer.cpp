#include "monochromeBuffer.h"
#include "../drivers/fullBufferScreenDriver.h"

namespace yoba {
	MonochromeBuffer::MonochromeBuffer(ScreenDriver* driver) : ScreenBuffer(driver) {

	}

	size_t MonochromeBuffer::getRequiredBufferLength() {
		return _driver->getResolution().getSquare() / 8;
	}

	void MonochromeBuffer::flush() {
		switch (_driver->getBufferType()) {
			case ScreenDriverBufferType::Full: {
//				printBufferContentsAsBinary();

				(dynamic_cast<FullBufferScreenDriver*>(_driver))->writePixels(_buffer);

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
//		const auto pixelIndex = getIndex(point);
//		const auto bufferIndex = pixelIndex / 8;
//		const auto bitIndex = pixelIndex % 8;
//
//		_buffer[bufferIndex] = ((_buffer[bufferIndex] & ~(1 << bitIndex)) | (((MonochromeColor*) color)->getValue() << bitIndex));

		if (((MonochromeColor*) color)->getValue()) {
			_buffer[point.getX() + (point.getY() / 8) * _driver->getResolution().getWidth()] |=  (1 << (point.getY() & 7));
		}
		else {
			_buffer[point.getX() + (point.getY() / 8) * _driver->getResolution().getWidth()] &= ~(1 << (point.getY() & 7));
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
		Serial.printf("Monochrome screen buffer %d x %d:\n", _driver->getResolution().getWidth(), _driver->getResolution().getHeight());

		size_t bufferPtr = 0;

		for (int j = 0; j < _driver->getResolution().getHeight(); j++) {
			for (int i = 0; i < _driver->getResolution().getWidth(); i += 8) {
				for (int k = 0; k < 8; k++) {
					Serial.print((((_buffer[bufferPtr] >> (7 - k)) & 0b1) == 1) ? "#" : ".");
				}

				bufferPtr++;
			}

			Serial.println();
		}
	}
}