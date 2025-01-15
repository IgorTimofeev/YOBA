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
}