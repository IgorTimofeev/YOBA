#include "bufferedScreenDriver.h"

namespace yoba {
	BufferedScreenDriver::BufferedScreenDriver() { // NOLINT(*-use-equals-default)

	}

	uint8_t BufferedScreenDriver::getPixelBufferHeight() const {
		return _pixelBufferHeight;
	}

	void BufferedScreenDriver::writePixelBuffer(const std::function<void(uint8_t*&, size_t&)>& pixelSetter) {
		size_t pixelIndex = 0;
		uint16_t y;
		uint8_t* pixelBufferStart;
		uint8_t* pixelBufferEnd = _pixelBuffer + _pixelBufferLength;

		for (y = 0; y < this->_resolution.getHeight(); y += _pixelBufferHeight) {
			pixelBufferStart = _pixelBuffer;

			while (pixelBufferStart < pixelBufferEnd) {
				pixelSetter(pixelBufferStart, pixelIndex);
			}

			writePixelBuffer(y);
		}
	}

	uint8_t* BufferedScreenDriver::getPixelBuffer() const {
		return _pixelBuffer;
	}

	size_t BufferedScreenDriver::getPixelBufferLength() const {
		return _pixelBufferLength;
	}

	void BufferedScreenDriver::updateFromOrientation() {
		ScreenDriver::updateFromOrientation();

		// Updating pixel buffer height
		_pixelBufferHeight = getPixelBufferHeightForOrientation();

		// Allocating pixel buffer
		delete _pixelBuffer;
		_pixelBufferLength = Color::getBytesPerType(this->_resolution.getWidth() * _pixelBufferHeight, _colorModel);
		_pixelBuffer = (uint8_t*) heap_caps_malloc(_pixelBufferLength, MALLOC_CAP_DMA);
		assert(_pixelBuffer != nullptr);
	}
}