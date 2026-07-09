#pragma once

#include <cstdlib>
#include <cstring>

#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>

namespace YOBA {
	class MonochromeBufferedRenderer : public BufferedRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;
			inline void clearNative(const Color* color) override;
			inline void putPixelNative(const Point& point, const Color* color) override;
			inline void strokeHorizontalLineNative(const Point& point, uint16_t width, const Color* color) override;
			inline void strokeVerticalLineNative(const Point& point, uint16_t height, const Color* color) override;
			inline void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			inline void putImageNative(const Point& point, const Image* image) override;
	};
}