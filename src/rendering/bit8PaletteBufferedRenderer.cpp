#include "bit8PaletteBufferedRenderer.h"
#include "bounds.h"
#include "hardware/displays/bufferedDisplay.h"

namespace yoba {
	using namespace yoba::hardware;

	Bit8PaletteBufferedRenderer::Bit8PaletteBufferedRenderer(RenderTarget* renderTarget, uint16_t paletteLength) : PaletteBufferedRenderer(renderTarget, paletteLength) {
		setPrimaryColor(&_primaryColor);
		setSecondaryColor(&_secondaryColor);
	}

	size_t Bit8PaletteBufferedRenderer::getRequiredBufferLength() {
		return getSize().getSquare();
	}

	void Bit8PaletteBufferedRenderer::flush() {
		switch (_renderTarget->getPixelWriting()) {
			case RenderTargetPixelWriting::Buffered: {
				const auto bufferedDisplay = dynamic_cast<BufferedDisplay*>(_renderTarget);

				switch (_renderTarget->getColorModel()) {
					case ColorModel::Rgb565: {
						bufferedDisplay->writePixelBuffer([&](uint8_t*& destination, size_t& pixelIndex) {
							((uint16_t*) destination)[0] = ((uint16_t*) _palette)[_buffer[pixelIndex]];
							destination += 2;
							pixelIndex++;
						});

						break;
					}
					case ColorModel::Rgb666: {
						const uint8_t* palettePtr;

						bufferedDisplay->writePixelBuffer([&](uint8_t*& destination, size_t& pixelIndex) {
							palettePtr = _palette + _buffer[pixelIndex] * 3;

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

	void Bit8PaletteBufferedRenderer::clearNative(const Color* color) {
		memset(_buffer, (int) getPaletteIndex(color), _bufferLength);
	}

	void Bit8PaletteBufferedRenderer::renderPixelNative(const Point& point, const Color* color) {
		_buffer[getIndex(point)] = getPaletteIndex(color);
	}

	void Bit8PaletteBufferedRenderer::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(_buffer + getIndex(point), getPaletteIndex(color), width);
	}

	void Bit8PaletteBufferedRenderer::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(point);
		uint16_t scanlineLength = getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteBufferedRenderer::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = _buffer + getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = getSize().getWidth();
		auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	void Bit8PaletteBufferedRenderer::renderImageNative(const Point& point, const Image* image) {
		size_t
			bufferIndex = getIndex(point),
			scanlineLength = getSize().getWidth() - image->getSize().getWidth(),
			imageIndex = 0;

		for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
			for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
				_buffer[bufferIndex] = image->getBitmap()[imageIndex];

				bufferIndex++;
				imageIndex++;
			}

			bufferIndex += scanlineLength;
		}
	}

	void Bit8PaletteBufferedRenderer::setOpenComputersPaletteColors() {
//		const uint8_t reds = 6;
//		const uint8_t greens = 8;
//		const uint8_t blues = 5;
//
//		for (uint8_t index = 0; index < 240; index++) {
//			const uint8_t idxB = index % blues;
//			const uint8_t idxG = (index / blues) % greens;
//			const uint8_t idxR = (index / blues / greens) % reds;
//
//			const auto r = (uint8_t) round((float) idxR * 255.0f / ((float) reds - 1.0f));
//			const auto g = (uint8_t) round((float) idxG * 255.0f / ((float) greens - 1.0f));
//			const auto b = (uint8_t) round((float) idxB * 255.0f / ((float) blues - 1.0f));
//
//			setPaletteColor(index, Rgb888Color(r, g, b));
//		}
//
//		for (uint8_t index = 0; index < 16; index++) {
//			const auto shade = (uint8_t) round(255.0f * (float) (index + 1) / 16.0f);
//
//			setPaletteColor(240 + index, Rgb888Color(shade, shade, shade));
//		}
	}
}