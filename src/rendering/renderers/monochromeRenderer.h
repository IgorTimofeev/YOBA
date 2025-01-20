#pragma once

#include <cstdlib>
#include <cstring>
#include "renderer.h"

namespace yoba {
	class MonochromeRenderer : public Renderer {
		public:
			void flushBuffer() override;

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