#include "bufferedRenderer.h"

namespace yoba {

	uint8_t* BufferedRenderer::getBuffer() const {
		return _buffer;
	}

	size_t BufferedRenderer::getBufferLength() const {
		return _bufferLength;
	}

	void BufferedRenderer::onTargetChanged() {
		Renderer::onTargetChanged();

		// (Re)allocating pixel buffer
		delete _buffer;

		if (getTarget()) {
			_bufferLength = getRequiredBufferLength();
			_buffer = new uint8_t[_bufferLength];
			assert(!!_buffer);
		}
		else {
			_bufferLength = 0;
		}
	}
}