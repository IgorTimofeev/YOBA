#include "bit8PaletteRenderer.h"
#include "bounds.h"
#include "rendering/targets/bufferedRenderTarget.h"

namespace yoba {
	using namespace yoba::hardware;

	Bit8PaletteRenderer::Bit8PaletteRenderer(uint16_t paletteLength) : PaletteRenderer(paletteLength) {

	}

	size_t Bit8PaletteRenderer::getRequiredBufferLength() {
		return getTarget()->getSize().getSquare();
	}

	void Bit8PaletteRenderer::flushBuffer() {
		switch (getTarget()->getPixelWriting()) {
			case RenderTargetPixelWriting::buffered: {
				const auto bufferedRenderTarget = dynamic_cast<BufferedRenderTarget*>(getTarget());

				switch (getTarget()->getColorModel()) {
					case ColorModel::rgb565: {
						bufferedRenderTarget->flushBuffer([&](uint8_t*& destination, size_t& pixelIndex) {
							((uint16_t*) destination)[0] = ((uint16_t*) getPalette())[getBuffer()[pixelIndex]];
							destination += 2;
							pixelIndex++;
						});

						break;
					}
					case ColorModel::rgb666: {
						const uint8_t* palettePtr;

						bufferedRenderTarget->flushBuffer([&](uint8_t*& destination, size_t& pixelIndex) {
							palettePtr = getPalette() + getBuffer()[pixelIndex] * 3;

							destination[0] = palettePtr[0];
							destination[1] = palettePtr[1];
							destination[2] = palettePtr[2];

							destination += 3;
							pixelIndex++;
						});

						break;
					}
					default:
						break;
				}

				break;
			}
			default:
				break;
		}
	}

	void Bit8PaletteRenderer::clearNative(const Color* color) {
		memset(getBuffer(), (int) getPaletteIndex(color), getBufferLength());
	}

	void Bit8PaletteRenderer::renderPixelNative(const Point& point, const Color* color) {
//		if (point.getX() < 0 || point.getX() >= 320 || point.getY() < 0 || point.getY() >= 240) {
//			Serial.printf("CYKA?? %d, %d\f", point.getX(), point.getY());
//			return;
//		}
//
//		if (getIndex(point) >= getBufferLength()) {
//			Serial.printf("getIndex POINT %d, %d\f", point.getX(), point.getY());
//			Serial.printf("getIndex INDEX %d, %d\f", getIndex(point), getBufferLength());
//			return;
//		}

		getBuffer()[getIndex(point)] = getPaletteIndex(color);
	}

	void Bit8PaletteRenderer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(getBuffer() + getIndex(point), getPaletteIndex(color), width);
	}

	void Bit8PaletteRenderer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = getBuffer() + getIndex(point);
		uint16_t scanlineLength = getTarget()->getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = getBuffer() + getIndex(bounds.getPosition());
		uint16_t scanlineLength = getTarget()->getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteRenderer::renderImageNative(const Point& point, const Image* image) {
		size_t
			bufferIndex = getIndex(point),
			scanlineLength = getTarget()->getSize().getWidth() - image->getSize().getWidth(),
			imageIndex = 0;

		for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
			for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
				getBuffer()[bufferIndex] = image->getBitmap()[imageIndex];

				bufferIndex++;
				imageIndex++;
			}

			bufferIndex += scanlineLength;
		}
	}

	void Bit8PaletteRenderer::setOpenComputersPaletteColors() {
		const uint8_t reds = 6;
		const uint8_t greens = 8;
		const uint8_t blues = 5;

		for (uint8_t index = 0; index < 240; index++) {
			const uint8_t idxB = index % blues;
			const uint8_t idxG = (index / blues) % greens;
			const uint8_t idxR = (index / blues / greens) % reds;

			const auto r = (uint8_t) std::round((float) idxR * 255.0f / ((float) reds - 1.0f));
			const auto g = (uint8_t) std::round((float) idxG * 255.0f / ((float) greens - 1.0f));
			const auto b = (uint8_t) std::round((float) idxB * 255.0f / ((float) blues - 1.0f));

			setPaletteColor(index, Rgb888Color(r, g, b));
		}

		for (uint8_t index = 0; index < 16; index++) {
			const auto shade = (uint8_t) std::round(255.0f * (float) (index + 1) / 16.0f);

			setPaletteColor(240 + index, Rgb888Color(shade, shade, shade));
		}
	}
}