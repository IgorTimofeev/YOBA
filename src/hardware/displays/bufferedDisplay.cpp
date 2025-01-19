#include "bufferedDisplay.h"

namespace yoba::hardware {
	BufferedDisplay::BufferedDisplay() { // NOLINT(*-use-equals-default)

	}

	uint8_t BufferedDisplay::getBufferHeight() const {
		return _bufferHeight;
	}

	void BufferedDisplay::writeBuffer(const std::function<void(uint8_t*&, size_t&)>& pixelSetter) {
		size_t pixelIndex = 0;
		uint16_t y;
		uint8_t* pixelBufferStart;
		uint8_t* pixelBufferEnd = _buffer + _bufferLength;

		for (y = 0; y < this->_size.getHeight(); y += _bufferHeight) {
			pixelBufferStart = _buffer;

			while (pixelBufferStart < pixelBufferEnd) {
				pixelSetter(pixelBufferStart, pixelIndex);
			}

			writeBuffer(y);
		}
	}

	uint8_t* BufferedDisplay::getBuffer() const {
		return _buffer;
	}

	size_t BufferedDisplay::getBufferLength() const {
		return _bufferLength;
	}

	void BufferedDisplay::updateFromOrientation() {
		RenderTarget::updateFromOrientation();

		// Updating pixel buffer height
		_bufferHeight = getBufferHeightForOrientation();

		// Allocating pixel buffer
		delete _buffer;
		_bufferLength = Color::getBytesPerType(this->_size.getWidth() * _bufferHeight, _colorModel);
		_buffer = (uint8_t*) heap_caps_malloc(_bufferLength, MALLOC_CAP_DMA);
		assert(_buffer != nullptr);
	}
}