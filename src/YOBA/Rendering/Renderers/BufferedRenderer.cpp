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

		_flushingChunkHeight = computeFlushingChunkHeight();
	}

	uint8_t* BufferedRenderer::getPixelBuffer() const {
		return _pixelBuffer;
	}

	size_t BufferedRenderer::getPixelBufferLength() const {
		return _pixelBufferLength;
	}

	int32_t BufferedRenderer::getPixelIndex(const int32_t x, const int32_t y) const {
		return y * getTarget()->getSize().getWidth() + x;
	}

	int32_t BufferedRenderer::getPixelIndex(const Point& point) const {
		return getPixelIndex(point.getX(), point.getY());
	}
	
	uint16_t BufferedRenderer::computeFlushingChunkHeight() const {
		#if defined(YOBA_SYSTEM_MCU)
			// On ESP32 S3 max transaction length is 32 kb

			const auto& size = getTarget()->getSize();
			const auto BPM = Color::getBytesPerModel(getTarget()->getColorModel());

			// Checking for most popular resolutions
			if (size.getWidth() == 80 && size.getHeight() == 160) {
				switch (BPM) {
					case 1: return size.getHeight();
					case 2: return size.getHeight();
					default: return size.getHeight() / 2;
				}
			}
			else if (size.getWidth() == 240 && size.getHeight() == 320) {
				switch (BPM) {
					case 1: return size.getHeight() / 4;
					case 2: {
						#if defined(CONFIG_IDF_TARGET_ESP32S3)
							return size.getHeight() / 5;
						#else
							return size.getHeight() / 4;
						#endif
					}
					default: return size.getHeight() / 10;
				}
			}
			else if (size.getWidth() == 320 && size.getHeight() == 480) {
				switch (BPM) {
					case 1: return size.getHeight() / 5;
					case 2: return size.getHeight() / 10;
					default: return size.getHeight() / 15;
				}
			}
			// Something retarded
			else {
				return getTarget()->getSize().getHeight() / 10;
			}
		#else
			return getTarget()->getSize().getHeight();
		#endif
	}

	uint16_t BufferedRenderer::getFlushingChunkHeight() const {
		return _flushingChunkHeight;
	}
}