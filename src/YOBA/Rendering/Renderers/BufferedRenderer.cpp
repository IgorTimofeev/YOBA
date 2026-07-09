#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void BufferedRenderer::reallocatePixelBuffer() {
		_pixelBufferLength = computePixelBufferLength();

		system::reallocate(_pixelBuffer, _pixelBufferLength);
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