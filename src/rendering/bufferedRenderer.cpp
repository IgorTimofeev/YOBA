#include "bufferedRenderer.h"

namespace yoba {
	BufferedRenderer::BufferedRenderer(RenderTarget* renderTarget) : Renderer(renderTarget) {

	}

	void BufferedRenderer::setup() {
		Renderer::setup();

		allocate();
	}

	void BufferedRenderer::allocate() {
		_bufferLength = getRequiredBufferLength();
		_buffer = new uint8_t[_bufferLength];
		assert(_buffer != nullptr);
	}

	uint8_t* BufferedRenderer::getBuffer() const {
		return _buffer;
	}

	size_t BufferedRenderer::getBufferLength() const {
		return _bufferLength;
	}
}