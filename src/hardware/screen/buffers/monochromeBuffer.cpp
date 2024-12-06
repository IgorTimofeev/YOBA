#include "monochromeBuffer.h"

namespace yoba {
	MonochromeBuffer::MonochromeBuffer(ScreenDriver* driver) : ScreenBuffer(driver) {

	}

	size_t MonochromeBuffer::getRequiredBufferLength() {
		return _driver->getResolution().getSquare() / 8;
	}

	void MonochromeBuffer::flush() {
		switch (_driver->getColorModel()) {
			case ColorModel::Monochrome:
				_driver->writePixels([&](uint8_t*& destination, size_t& pixelIndex) {
					destination[0] = _buffer[pixelIndex / 8];
					destination++;
					pixelIndex += 8;
				});

				break;

			default:
				break;
		}
	}

	void MonochromeBuffer::clearNative(const Color* color) {
		const auto monochromeColor = (MonochromeColor*) color;

		uint8_t byteValue =
			(monochromeColor->getValue() << 7) |
			(monochromeColor->getValue() << 6) |
			(monochromeColor->getValue() << 5) |
			(monochromeColor->getValue() << 4) |
			(monochromeColor->getValue() << 3) |
			(monochromeColor->getValue() << 2) |
			(monochromeColor->getValue() << 1) |
			monochromeColor->getValue();

		memset(_buffer, byteValue, _bufferLength);
	}

	void MonochromeBuffer::renderPixelNative(const Point& point, const Color* color) {
		const auto monochromeColor = (MonochromeColor*) color;
		const auto bufferIndex = getIndex(point);
		const uint8_t bitIndex = bufferIndex % 8;

		auto value = _buffer[bufferIndex];
		auto newValue = ((value & ~(1 << bitIndex)) | (monochromeColor->getValue() << bitIndex));

		_buffer[bufferIndex] = newValue;
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
}