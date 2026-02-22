#pragma once

#include <cstdlib>
#include <cstring>
#include <YOBA/main/rendering/pixelBufferRenderer.h>

namespace YOBA {
	class MonochromeRenderer : public PixelBufferRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;
			inline void clearNative(const Color& color) override;
			inline void renderPixelNative(const Point& point, const Color& color) override;
			inline void renderHorizontalLineNative(const Point& point, uint16_t width, const Color& color) override;
			inline void renderVerticalLineNative(const Point& point, uint16_t height, const Color& color) override;
			inline void renderFilledRectangleNative(const Bounds& bounds, const Color& color) override;
			inline void renderImageNative(const Point& point, const Image& image) override;
	};
}