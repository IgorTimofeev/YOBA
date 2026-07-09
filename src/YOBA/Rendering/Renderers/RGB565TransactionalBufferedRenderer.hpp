#pragma once

#include <YOBA/Rendering/Renderers/TransactionalBufferedRenderer.hpp>

namespace YOBA {
	class RGB565TransactionalBufferedRenderer : public TransactionalBufferedRenderer {
		public:
			void flush() override;

		protected:
			size_t computePixelBufferLength() const override;
			void clearNative(const Color* color) override;
			void putPixelNative(const Point& point, const Color* color) override;
			void strokeHorizontalLineNative(const Point& point, uint16_t length, const Color* color) override;
			void strokeVerticalLineNative(const Point& point, uint16_t length, const Color* color) override;
			void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			void putImageNative(const Point& point, const Image* image) override;
	};
}