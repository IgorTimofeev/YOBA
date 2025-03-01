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

	void BufferedRenderTarget::flushBuffer(uint16_t width, const std::function<void(uint8_t*&, uint32_t&)>& pixelSetter) {
		uint32_t pixelIndex = 0;
		uint16_t y;
		uint8_t* pixelBufferStart;
		uint8_t* pixelBufferEnd = _buffer + _bufferLength;

		for (y = 0; y < this->_size.getHeight(); y += _bufferHeight) {
			pixelBufferStart = _buffer;

			while (pixelBufferStart < pixelBufferEnd) {
				pixelSetter(pixelBufferStart, pixelIndex);
			}

			flushBuffer(Bounds(0, y, width, _bufferHeight), _bufferLength);
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

		// Reallocating pixel buffer
		delete _buffer;

		_bufferLength = Color::getBytesPerModel(this->_size.getWidth() * _bufferHeight, _colorModel);

		#ifdef ESP_PLATFORM
			_buffer = (uint8_t*) heap_caps_malloc(_bufferLength, MALLOC_CAP_DMA);
			assert(_buffer != nullptr);
		#else
			_buffer = new uint8_t[_bufferLength];
		#endif
	}
}