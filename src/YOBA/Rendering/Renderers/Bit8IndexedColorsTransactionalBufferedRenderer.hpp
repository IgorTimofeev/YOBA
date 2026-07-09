#pragma once

#include <YOBA/Rendering/Renderers/IndexedColorsTransactionalBufferedRenderer.hpp>

namespace YOBA {
	class Bit8IndexedColorsTransactionalBufferedRenderer : public IndexedColorsTransactionalBufferedRenderer<uint8_t, uint16_t> {
		public:
			explicit Bit8IndexedColorsTransactionalBufferedRenderer(uint8_t paletteLength);

			void flush() override;

			// Original generation algo can be found here:
			// https://github.com/MightyPirates/OpenComputers/blob/49ae4fe850e25e8eb98e62b2ac0abefaf8893102/src/main/scala/li/cil/oc/util/PackedColor.scala#L124-L141
			void setOpenComputersPaletteColors();

		protected:
			size_t computePixelBufferLength() const override;
			size_t computePaletteIndicesBufferLength() const override;

			inline void clearNative(const Color* color) override;
			inline void putPixelNative(const Point& point, const Color* color) override;
			inline void strokeHorizontalLineNative(const Point& point, uint16_t width, const Color* color) override;
			inline void strokeVerticalLineNative(const Point& point, uint16_t height, const Color* color) override;
			inline void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			inline void putImageNative(const Point& point, const Image* image) override;
	};
}