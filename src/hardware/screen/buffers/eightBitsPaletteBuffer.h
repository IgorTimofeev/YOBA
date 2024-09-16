#pragma once

#include "paletteBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	class EightBitsPaletteBuffer : public PaletteBuffer {
		public:
			explicit EightBitsPaletteBuffer(ScreenDriver *driver);

			void allocate() override;
			void flush() override;

			// Original generation algo can be found here:
			// https://github.com/MightyPirates/OpenComputers/blob/49ae4fe850e25e8eb98e62b2ac0abefaf8893102/src/main/scala/li/cil/oc/util/PackedColor.scala#L124-L141
			void fillWithOpenComputersColors();

		protected:
			void renderPixelUnchecked(const Point &point, const Color* color) override;
			void renderHorizontalLineUnchecked(const Point &point, uint16_t width, const Color* color) override;
			void renderVerticalLineUnchecked(const Point &point, uint16_t height, const Color* color) override;
			void renderFilledRectangleUnchecked(const Bounds& bounds, const Color* color) override;

		private:
			uint16_t _govnoPalette[256] {};
	};
}