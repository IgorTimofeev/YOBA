#pragma once

#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>

namespace YOBA {
	class ARGBBufferedRenderer : public BufferedRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;

			void clearNative(const Color* color) override;
			void putPixelNative(const Point& point, const Color* color) override;
			void strokeHorizontalLineNative(const Point& point, const uint16_t length, const Color* color) override;
			void strokeVerticalLineNative(const Point& point, const uint16_t length, const Color* color) override;
			void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			void putImageNative(const Point& position, const Image* image) override;
	};
}