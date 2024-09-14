#pragma once

#include "paletteBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	class EightBitsPaletteBuffer : public PaletteBuffer<uint8_t> {
		public:
			EightBitsPaletteBuffer(ScreenDriver *driver, const Size &resolution);

			void allocate() override;
			void flush() override;

		protected:
			uint8_t getNativeColor(Color* color) override;
			void renderPixelNative(const Point &point, uint8_t paletteIndex) override;
			void renderHorizontalLineNative(const Point &point, uint16_t width, uint8_t paletteIndex) override;
			void renderVerticalLineNative(const Point &point, uint16_t height, uint8_t paletteIndex) override;
			void renderFilledRectangleNative(const Bounds& bounds, uint8_t paletteIndex) override;

		private:
			uint16_t _palette[256] {};
	};
}