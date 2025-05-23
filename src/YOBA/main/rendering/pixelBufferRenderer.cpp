#include <YOBA/main/rendering/pixelBufferRenderer.h>

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace YOBA {
	void PixelBufferRenderer::reallocatePixelBuffer() {
		if (_pixelBuffer)
			heap_caps_free(_pixelBuffer);

		_pixelBufferLength = computePixelBufferLength();

		#ifdef ESP_PLATFORM
			_pixelBuffer = static_cast<uint8_t*>(heap_caps_malloc(_pixelBufferLength, MALLOC_CAP_DMA));
			assert(_pixelBuffer != nullptr);
		#else
			_pixelBuffer = new uint8_t[_pixelBufferLength];
		#endif
	}

	void PixelBufferRenderer::updateFromTarget() {
		Renderer::updateFromTarget();

		reallocatePixelBuffer();
	}

	uint8_t* PixelBufferRenderer::getPixelBuffer() const {
		return _pixelBuffer;
	}

	size_t PixelBufferRenderer::getPixelBufferLength() const {
		return _pixelBufferLength;
	}

	size_t PixelBufferRenderer::getPixelIndex(uint16_t x, uint16_t y) const {
		return y * getTarget()->getSize().getWidth() + x;
	}

	size_t PixelBufferRenderer::getPixelIndex(const Point& point) const {
		return getPixelIndex(point.getX(), point.getY());
	}
}