#pragma once

#include <cstdlib>
#include <cstdint>
#include "renderTarget.h"
#include "size.h"
#include "bounds.h"
#include "color.h"
#include "image.h"
#include "font.h"

namespace yoba {
	class Renderer {
		public:
			explicit Renderer(RenderTarget* renderTarget);

			virtual void setup();

			RenderTarget* getRenderTarget() const;

			const Bounds& getViewport();
			void setViewport(const Bounds& viewport);

			Bounds pushViewport(const Bounds& bounds);
			void popViewport(const Bounds& bounds);

			void resetViewport();

			size_t getIndex(uint16_t x, uint16_t y) const;
			size_t getIndex(const Point& point) const;

			// -------------------------------- Native rendering --------------------------------

			void clear(const Color* color);
			void renderPixel(const Point& point, const Color* color);
			void renderHorizontalLine(const Point& point, uint16_t length, const Color* color);
			void renderVerticalLine(const Point& point, uint16_t length, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color);
			void renderImage(const Point& point, const Image* image);

			// -------------------------------- Non-native rendering --------------------------------

			void renderRectangle(const Bounds& bounds, const Color* color);
			void renderRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color);

			// Thanks, AdaFruit!
			void renderLine(const Point& from, const Point& to, const Color* color);
			void renderTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);
			void renderCircle(const Point& center, uint16_t radius, const Color* color);
			void renderFilledTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);
			void renderFilledCircle(const Point& center, uint16_t radius, const Color* color);

			/**
			* @brief Renders single line of text
			* @param point Position of upper-left corner of the first character in text
			* @param font Font with which the text will be rendered
			* @param color Color with which the text will be rendered
			* @param string Template-based pointer to first character in text
			*/
			void renderString(const Point& point, const Font* font, const Color* color, const std::basic_string_view<wchar_t>& string, uint8_t fontScale = 1);
			void renderChar(const Point& point, const Font* font, const Color* color, wchar_t ch, uint8_t fontScale = 1);

			virtual void flush() = 0;

		protected:
			virtual void clearNative(const Color* color) = 0;
			virtual void renderPixelNative(const Point& point, const Color* color) = 0;
			virtual void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, const Color* color) = 0;
			virtual void renderImageNative(const Point& point, const Image* image) = 0;

		private:
			RenderTarget* _renderTarget;
			Bounds _viewport = Bounds();

			void renderRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color);
			void renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color* color);
			void renderMissingGlyph(const Point& point, const Font* font, const Color* color, uint8_t fontScale);
			inline void renderGlyph(const Point& point, const Font* font, const Color* color, const Glyph* glyph, uint8_t fontScale);
	};
}