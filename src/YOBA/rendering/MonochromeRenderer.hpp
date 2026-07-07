#pragma once

#include <cstdlib>
#include <cstring>

#include <YOBA/Rendering/BufferedRenderer.hpp>

namespace YOBA {
	class MonochromeRenderer : public BufferedRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;
			inline void clearNative(const Color* color) override;
			inline void renderPixelNative(const Point& point, const Color* color) override;
			inline void renderHorizontalLineNative(const Point& point, uint16_t width, const Color* color) override;
			inline void renderVerticalLineNative(const Point& point, uint16_t height, const Color* color) override;
			inline void renderFilledRectangleNative(const Rectangle& bounds, const Color* color) override;
			inline void renderImageNative(const Point& point, const Image* image) override;
	};
}