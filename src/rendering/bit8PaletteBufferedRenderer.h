#pragma once

#include "paletteBufferedRenderer.h"
#include "renderer.h"

namespace yoba {
	class Bit8PaletteBufferedRenderer : public PaletteBufferedRenderer {
		public:
			explicit Bit8PaletteBufferedRenderer(uint16_t paletteLength);

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
}