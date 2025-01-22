#include "bufferedRenderTarget.h"

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace yoba::hardware {
	BufferedRenderTarget::BufferedRenderTarget() { // NOLINT(*-use-equals-default)

	}

	uint8_t BufferedRenderTarget::getBufferHeight() const {
		return _bufferHeight;
	}

	void BufferedRenderTarget::flushBuffer(const std::function<void(uint8_t*&, size_t&)>& pixelSetter) {
		size_t pixelIndex = 0;
		uint16_t y;
		uint8_t* pixelBufferStart;
		uint8_t* pixelBufferEnd = _buffer + _bufferLength;

		for (y = 0; y < this->_size.getHeight(); y += _bufferHeight) {
			pixelBufferStart = _buffer;

			while (pixelBufferStart < pixelBufferEnd) {
				pixelSetter(pixelBufferStart, pixelIndex);
			}

			flushBuffer(y);
		}
	}

	uint8_t* BufferedRenderTarget::getBuffer() const {
		return _buffer;
	}

	size_t BufferedRenderTarget::getBufferLength() const {
		return _bufferLength;
	}

	void BufferedRenderTarget::updateFromOrientation() {
		RenderTarget::updateFromOrientation();

		// Updating pixel buffer height
		_bufferHeight = getBufferHeightForOrientation();

		// Allocating pixel buffer
		delete _buffer;
		_bufferLength = Color::getBytesPerType(this->_size.getWidth() * _bufferHeight, _colorModel);

		#ifdef ESP_PLATFORM
			_buffer = (uint8_t*) heap_caps_malloc(_bufferLength, MALLOC_CAP_DMA);
		#else
			_buffer = new uint8_t[_bufferLength];
		#endif

		assert(_buffer != nullptr);
	}
}