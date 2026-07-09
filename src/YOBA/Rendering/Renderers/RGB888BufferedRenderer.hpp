#pragma once

#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>

namespace YOBA {
	class RGB888BufferedRenderer : public BufferedRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;

			void clearNative(const Color* color) override;
			void renderPixelNative(const Point& point, const Color* color) override;
			void renderHorizontalLineNative(const Point& point, const uint16_t length, const Color* color) override;
			void renderVerticalLineNative(const Point& point, const uint16_t length, const Color* color) override;
			void renderFilledRectangleNative(const Rectangle& bounds, const Color* color) override;
			void renderImageNative(const Point& point, const Image* image) override;
	};
}