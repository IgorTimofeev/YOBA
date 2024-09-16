#pragma once

#include "paletteBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	class Bits16Colors256PaletteBuffer : public PaletteBuffer<uint8_t, uint16_t, 256>{
		public:
			explicit Bits16Colors256PaletteBuffer(ScreenDriver *driver);

			void allocate() override;
			void flush() override;

			// Original generation algo can be found here:
			// https://github.com/MightyPirates/OpenComputers/blob/49ae4fe850e25e8eb98e62b2ac0abefaf8893102/src/main/scala/li/cil/oc/util/PackedColor.scala#L124-L141
			void setOpenComputersPaletteColors();

		protected:
			void renderPixelNative(const Point &point, const Color* color) override;
			void renderHorizontalLineNative(const Point &point, uint16_t width, const Color* color) override;
			void renderVerticalLineNative(const Point &point, uint16_t height, const Color* color) override;
			void renderFilledRectangleNative(const Bounds& bounds, const Color* color) override;
	};
}