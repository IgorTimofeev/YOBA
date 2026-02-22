#pragma once

#include <YOBA/main/rendering/transactionalPixelBufferRenderer.h>

namespace YOBA {
	class RGB565PixelBufferRenderer : public TransactionalPixelBufferRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;
			void clearNative(const Color& color) override;
			void renderPixelNative(const Point& point, const Color& color) override;
			void renderHorizontalLineNative(const Point& point, uint16_t length, const Color& color) override;
			void renderVerticalLineNative(const Point& point, uint16_t length, const Color& color) override;
			void renderFilledRectangleNative(const Bounds& bounds, const Color& color) override;
			void renderImageNative(const Point& point, const Image& image) override;
	};
}