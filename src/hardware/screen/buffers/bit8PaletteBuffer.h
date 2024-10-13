#pragma once

#include "paletteBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	template<typename TColor, size_t PaletteLength>
	class Bit8PaletteBuffer : public PaletteBuffer<uint8_t, TColor, PaletteLength>{
		public:
			explicit Bit8PaletteBuffer(ScreenDriver* driver);

			void flush() override;

			// Original generation algo can be found here:
			// https://github.com/MightyPirates/OpenComputers/blob/49ae4fe850e25e8eb98e62b2ac0abefaf8893102/src/main/scala/li/cil/oc/util/PackedColor.scala#L124-L141
			void setOpenComputersPaletteColors();

		protected:
			size_t getRequiredBufferLength() override;
			inline void clearNative(const Color* color) override;
			inline void renderPixelNative(const Point& point, const Color* color) override;
			inline void renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) override;
			inline void renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) override;
			inline void renderFilledRectangleNative(const Bounds& bounds, const Color* color) override;
			inline void renderImageNative(const Point& point, const Image* image) override;
	};

	template<typename TColor, size_t PaletteLength>
	Bit8PaletteBuffer<TColor, PaletteLength>::Bit8PaletteBuffer(ScreenDriver* driver) : PaletteBuffer<uint8_t, TColor, PaletteLength>(driver) {

	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::flush() {
		this->_driver->writePixels([&](size_t pixelIndex) {
			return this->_palette[this->_buffer[pixelIndex]];
		});
	}

	template<typename TColor, size_t PaletteLength>
	size_t Bit8PaletteBuffer<TColor, PaletteLength>::getRequiredBufferLength() {
		return this->_driver->getResolution().getWidth() * this->_driver->getResolution().getHeight();
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::clearNative(const Color* color) {
		memset(this->_buffer, (int) this->getPaletteIndex(color), this->_bufferLength);
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::renderPixelNative(const Point& point, const Color* color) {
		this->_buffer[this->getIndex(point)] = this->getPaletteIndex(color);
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) {
		memset(this->_buffer + this->getIndex(point), this->getPaletteIndex(color), width);
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) {
		uint8_t* bufferPtr = this->_buffer + this->getIndex(point);
		uint16_t scanlineLength = this->_driver->getResolution().getWidth();
		auto paletteIndex = this->getPaletteIndex(color);

		for (size_t i = 0; i < height; i++) {
			memset(bufferPtr, paletteIndex, 1);
			bufferPtr += scanlineLength;
		}
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::renderFilledRectangleNative(const Bounds& bounds, const Color* color) {
		uint8_t* bufferPtr = this->_buffer + this->getIndex(bounds.getTopLeft());
		uint16_t scanlineLength = this->_driver->getResolution().getWidth();
		auto paletteIndex = this->getPaletteIndex(color);

		for (uint16_t i = 0; i < bounds.getHeight(); i++) {
			memset(bufferPtr, paletteIndex, bounds.getWidth());
			bufferPtr += scanlineLength;
		}
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::renderImageNative(const Point& point, const Image* image) {
		size_t
			bufferIndex = this->getIndex(point),
			scanlineLength = this->_driver->getResolution().getWidth() - image->getSize().getWidth(),
			imageIndex = 0;

		for (uint16_t y = 0; y < image->getSize().getHeight(); y++) {
			for (uint16_t x = 0; x < image->getSize().getWidth(); x++) {
				this->_buffer[bufferIndex] = image->getBitmap()[imageIndex];

				bufferIndex++;
				imageIndex++;
			}

			bufferIndex += scanlineLength;
		}
	}

	template<typename TColor, size_t PaletteLength>
	void Bit8PaletteBuffer<TColor, PaletteLength>::setOpenComputersPaletteColors() {
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
//			this->setPaletteColor(index, Rgb888Color(r, g, b));
//		}
//
//		for (uint8_t index = 0; index < 16; index++) {
//			const auto shade = (uint8_t) round(255.0f * (float) (index + 1) / 16.0f);
//
//			this->setPaletteColor(240 + index, Rgb888Color(shade, shade, shade));
//		}
	}

	template<size_t PaletteLength>
	using Rgb565Bit8PaletteBuffer = Bit8PaletteBuffer<uint16_t, PaletteLength>;

	template<size_t PaletteLength>
	using Rgb666Bit8PaletteBuffer = Bit8PaletteBuffer<uint32_t, PaletteLength>;
}