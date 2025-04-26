#pragma once

#include "YOBA/main/rendering/pixelBufferRenderer.h"

namespace YOBA {
	class RGB565PixelBufferRenderer : public PixelBufferRenderer {
		public:
			void flush() override;
			size_t getPixelBufferIndex(uint16_t x, uint16_t y) const override;

		protected:
			size_t computePixelBufferLength() const override;
			void updateFromTarget() override;
			void clearNative(const Color* color) override;
			void renderPixelNative(const Point& point, const Color* color) override;
			void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) override;
			void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) override;
			void renderFilledRectangleNative(const Bounds& bounds, const Color* color) override;
			void renderImageNative(const Point& point, const Image* image) override;
	};
}