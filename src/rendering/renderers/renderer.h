#pragma once

#include <cstdlib>
#include <cstdint>
#include "rendering/targets/renderTarget.h"
#include "main/size.h"
#include "main/bounds.h"
#include "main/color.h"
#include "main/image.h"
#include "main/font.h"

namespace yoba {
	class Renderer {
		public:
			RenderTarget* getTarget() const;
			void setTarget(RenderTarget* value);

			const Bounds& getViewport();
			void setViewport(const Bounds& viewport);
			Bounds pushViewport(const Bounds& bounds);
			void popViewport(const Bounds& bounds);
			void resetViewport();

			size_t getIndex(uint16_t x, uint16_t y) const;
			size_t getIndex(const Point& point) const;

			void clear(const Color* color);
			void renderPixel(const Point& point, const Color* color);
			void renderHorizontalLine(const Point& point, uint16_t length, const Color* color);
			void renderVerticalLine(const Point& point, uint16_t length, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color);
			void renderDitheredRectangle(const Bounds& bounds, const Color* color, uint8_t dotSize = 1, uint8_t transparencySize = 1);

			void renderImage(const Point& point, const Image* image);
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
			* @param fontScale Scale factor of text, defaults to 1
			*/
			void renderString(const Point& point, const Font* font, const Color* color, std::wstring_view string, uint8_t fontScale = 1);
			void renderChar(const Point& point, const Font* font, const Color* color, wchar_t ch, uint8_t fontScale = 1);

			virtual void flushBuffer() = 0;

		protected:
			virtual size_t getRequiredBufferLength() = 0;

			virtual void onTargetChanged();

			virtual void clearNative(const Color* color) = 0;
			virtual void renderPixelNative(const Point& point, const Color* color) = 0;
			virtual void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, const Color* color) = 0;
			virtual void renderImageNative(const Point& point, const Image* image) = 0;

			uint8_t* getBuffer() const;
			size_t getBufferLength() const;

		private:
			RenderTarget* _target;
			Bounds _viewport = Bounds();

			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;

			void renderRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color);
			void renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color* color);
			void renderMissingGlyph(const Point& point, const Font* font, const Color* color, uint8_t fontScale);
			inline void renderGlyph(const Point& point, const Font* font, const Color* color, const Glyph* glyph, uint8_t fontScale);
	};
}