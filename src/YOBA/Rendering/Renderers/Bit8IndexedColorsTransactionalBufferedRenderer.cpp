#include <YOBA/Rendering/Renderers/Bit8IndexedColorsTransactionalBufferedRenderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	Bit8IndexedColorsTransactionalBufferedRenderer::Bit8IndexedColorsTransactionalBufferedRenderer(const uint8_t paletteLength) : IndexedColorsTransactionalBufferedRenderer(paletteLength) {

	}

	size_t Bit8IndexedColorsTransactionalBufferedRenderer::computePixelBufferLength() const {
		return
			getTarget()->getSize().getWidth()
			* getTransactionViewportHeight()
			* Color::getBytesPerModel(getTarget()->getColorModel());
	}

	size_t Bit8IndexedColorsTransactionalBufferedRenderer::computePaletteIndicesBufferLength() const {
		return getTarget()->getSize().getSquare();
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::flush() {
		switch (getTarget()->getColorModel()) {
			case ColorModel::RGB565: {
				const auto& size = getTarget()->getSize();

				const uint16_t* transactionBufferEndPtr = reinterpret_cast<uint16_t*>(_pixelBuffer + _pixelBufferLength);

				const uint16_t* palettePtr = reinterpret_cast<uint16_t*>(getPalette());
				const uint8_t* paletteIndicesBufferPtr = _paletteIndicesBuffer;

				for (uint16_t y = 0; y < size.getHeight(); y += _transactionViewportHeight) {
					auto transactionBufferPtr = reinterpret_cast<uint16_t*>(_pixelBuffer);

					// Taking indices from palette, converting them to color & copying to pixel buffer
					while (transactionBufferPtr < transactionBufferEndPtr) {
						*transactionBufferPtr = palettePtr[*paletteIndicesBufferPtr];
						transactionBufferPtr++;
						paletteIndicesBufferPtr++;
					}

					// Writing pixel buffer on target
					getTarget()->flush(
						Rectangle(0, y, size.getWidth(), _transactionViewportHeight),
						{ _pixelBuffer, _pixelBufferLength }
					);
				}

				break;
			}
			// TODO: move this to some sort of RGB666Bit8PaletteRenderer
			case ColorModel::RGB666: {
				const auto& size = getTarget()->getSize();

				const uint8_t* pixelBufferEndPtr = _pixelBuffer + _pixelBufferLength;

				const uint8_t* paletteIndicesBufferPtr = _paletteIndicesBuffer;

				for (uint16_t y = 0; y < size.getHeight(); y += getTransactionViewportHeight()) {
					auto pixelBufferPtr = _pixelBuffer;

					// Taking indices from palette, converting them to color & copying to pixel buffer
					while (pixelBufferPtr < pixelBufferEndPtr) {
						std::memcpy(pixelBufferPtr, getPalette() + *paletteIndicesBufferPtr, 3);
						paletteIndicesBufferPtr++;
						pixelBufferPtr += 3;
					}

					// Writing pixel buffer on target
					getTarget()->flush(
						Rectangle(0, y, size.getWidth(), getTransactionViewportHeight()),
						{ _pixelBuffer, _pixelBufferLength }
					);
				}

				break;
			}
			default:
				break;
		}
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::clearNative(const Color* color) {
		std::memset(_paletteIndicesBuffer, getPaletteIndex(color), _paletteIndicesBufferLength);
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::putPixelNative(const Point& point, const Color* color) {
		_paletteIndicesBuffer[getPixelIndex(point)] = getPaletteIndex(color);
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::strokeHorizontalLineNative(const Point& point, const uint16_t width, const Color* color) {
		std::memset(_paletteIndicesBuffer + getPixelIndex(point), getPaletteIndex(color), width);
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::strokeVerticalLineNative(const Point& point, const uint16_t height, const Color* color) {
		uint8_t* paletteIndicesBufferPtr = _paletteIndicesBuffer + getPixelIndex(point);
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < height; i++) {
			*paletteIndicesBufferPtr = paletteIndex;
			paletteIndicesBufferPtr += scanlineLength;
		}
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		uint8_t* paletteIndicesBufferPtr = _paletteIndicesBuffer + getPixelIndex(bounds.getPosition());
		const uint16_t scanlineLength = getTarget()->getSize().getWidth();
		const auto paletteIndex = getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			std::memset(paletteIndicesBufferPtr, paletteIndex, bounds.getWidth());
			paletteIndicesBufferPtr += scanlineLength;
		}
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::putImageNative(const Point& point, const Image* image) {
		if (image->getColorModel() != ColorModel::indexed8Bit)
			return;

		auto paletteIndicesBufferPtr = _paletteIndicesBuffer + getPixelIndex(point);
		auto bitmapPtr = image->getBitmap();

		const auto scanlineLength = getTarget()->getSize().getWidth() - image->getSize().getWidth();

		// With alpha
		if (image->getOptions() & ImageOptions::alpha1Bit) {
			uint8_t bitmapBitIndex = 0;

			// 0000 0000|0000 0000
			// ---- -+--|---- -2--
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					// Non-transparent
					if (*bitmapPtr & 1 << bitmapBitIndex) {
						bitmapBitIndex++;

						// Easy
						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;

							*paletteIndicesBufferPtr = *bitmapPtr;

							paletteIndicesBufferPtr++;
							bitmapPtr++;
						}
						// Dark souls
						else {
							*paletteIndicesBufferPtr = static_cast<uint8_t>(*reinterpret_cast<const uint16_t*>(bitmapPtr) >> bitmapBitIndex & 0xFF);

							paletteIndicesBufferPtr++;
							bitmapPtr++;
						}
					}
					// Transparent
					else {
						bitmapBitIndex++;

						if (bitmapBitIndex > 7) {
							bitmapBitIndex = 0;
							bitmapPtr++;
						}

						paletteIndicesBufferPtr++;
					}
				}

				paletteIndicesBufferPtr += scanlineLength;
			}
		}
		// Without
		else {
			for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
				for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
					*paletteIndicesBufferPtr = *bitmapPtr;

					paletteIndicesBufferPtr++;
					bitmapPtr++;
				}

				paletteIndicesBufferPtr += scanlineLength;
			}
		}
	}

	void Bit8IndexedColorsTransactionalBufferedRenderer::setOpenComputersPaletteColors() {
		constexpr uint8_t reds = 6;
		constexpr uint8_t greens = 8;
		constexpr uint8_t blues = 5;

		for (uint8_t index = 0; index < 240; index++) {
			const uint8_t idxB = index % blues;
			const uint8_t idxG = index / blues % greens;
			const uint8_t idxR = index / blues / greens % reds;

			const auto r = static_cast<uint8_t>(std::round(static_cast<float>(idxR) * 255.0f / (static_cast<float>(reds) - 1.0f)));
			const auto g = static_cast<uint8_t>(std::round(static_cast<float>(idxG) * 255.0f / (static_cast<float>(greens) - 1.0f)));
			const auto b = static_cast<uint8_t>(std::round(static_cast<float>(idxB) * 255.0f / (static_cast<float>(blues) - 1.0f)));

			setPaletteColor(index, RGB888Color(r, g, b));
		}

		for (uint8_t index = 0; index < 16; index++) {
			const auto shade = static_cast<uint8_t>(std::round(255.0f * static_cast<float>(index + 1) / 16.0f));

			setPaletteColor(240 + index, RGB888Color(shade, shade, shade));
		}
	}
}