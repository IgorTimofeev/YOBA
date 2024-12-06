#pragma once

#include <cstdlib>
#include <cstring>
#include "screenBuffer.h"

namespace yoba {
	class MonochromeBuffer : public ScreenBuffer {
		public:
			explicit MonochromeBuffer(ScreenDriver* driver);

			void flush() override;

			void printBufferContentsAsBinary();

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