#include <YOBA/rendering/bufferedRenderer.h>

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace YOBA {
	void BufferedRenderer::reallocatePixelBuffer() {
		if (_pixelBuffer) {
			#ifdef ESP_PLATFORM
				heap_caps_free(_pixelBuffer);
			#else
				delete _pixelBuffer;
			#endif
		}

		_pixelBufferLength = computePixelBufferLength();

		#ifdef ESP_PLATFORM
			_pixelBuffer = static_cast<uint8_t*>(heap_caps_malloc(_pixelBufferLength, MALLOC_CAP_DMA));
			assert(_pixelBuffer != nullptr);
		#else
			_pixelBuffer = new uint8_t[_pixelBufferLength];
		#endif
	}

	void BufferedRenderer::updateFromTarget() {
		Renderer::updateFromTarget();

		reallocatePixelBuffer();
	}

	uint8_t* BufferedRenderer::getPixelBuffer() const {
		return _pixelBuffer;
	}

	size_t BufferedRenderer::getPixelBufferLength() const {
		return _pixelBufferLength;
	}

	size_t BufferedRenderer::getPixelIndex(const uint16_t x, const uint16_t y) const {
		return y * getTarget()->getSize().getWidth() + x;
	}

	size_t BufferedRenderer::getPixelIndex(const Point& point) const {
		return getPixelIndex(point.getX(), point.getY());
	}
}