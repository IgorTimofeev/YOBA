#pragma once

#include <cstdlib>
#include <cstdint>
#include <YOBA/main/rendering/renderTarget.h>
#include <YOBA/main/size.h>
#include <YOBA/main/bounds.h>
#include <YOBA/main/color.h>
#include <YOBA/main/image.h>
#include <YOBA/main/font.h>

namespace YOBA {
	class Renderer {
		friend class RenderTarget;

		public:
			virtual ~Renderer() = default;

			RenderTarget* getTarget() const;
			void setTarget(RenderTarget* value);

			const Bounds& getViewport() const;
			void setViewport(const Bounds& viewport);
			Bounds pushViewport(const Bounds& bounds);
			void popViewport(const Bounds& bounds);
			void resetViewport();

			void clear(const Color* color);
			void renderPixel(const Point& point, const Color* color);
			void renderHorizontalLine(const Point& point, uint16_t length, const Color* color);
			void renderVerticalLine(const Point& point, uint16_t length, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, const Color* color);
			void renderFilledRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color);
			void renderDitheredRectangle(const Bounds& bounds, const Color* color);
			void renderFilledQuad(const Point& topLeft, const Point& topRight, const Point& bottomRight, const Point& bottomLeft, const Color* color);

			void renderImage(const Point& point, const Image* image);
			void renderRectangle(const Bounds& bounds, const Color* color);
			void renderRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color);
			// Thanks, AdaFruit!
			void renderLine(const Point& from, const Point& to, const Color* color);
			void renderLine(const Point& from, const Point& to, const Color* color, uint8_t thickness);
			void renderTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);
			void renderCircle(const Point& center, uint16_t radius, const Color* color);
			void renderFilledTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);
			void renderFilledCircle(const Point& center, uint16_t radius, const Color* color);
			// Thanks, u8g2!
			void renderArc(const Point& center, uint16_t radius, float startAngleRad, float endAngleRad, const Color* color);
			void renderCatmullRomSpline(const Point* points, const size_t pointsLength, const Color* color, const uint16_t segmentsPerCurve = 10, const float tension = 0.5f);

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

			virtual void flush() = 0;

		protected:
			virtual void updateFromTarget();
			virtual void clearNative(const Color* color) = 0;
			virtual void renderPixelNative(const Point& point, const Color* color) = 0;
			virtual void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, const Color* color) = 0;
			virtual void renderImageNative(const Point& point, const Image* image) = 0;

		private:
			RenderTarget* _target = nullptr;
			Bounds _viewport {};

			static Point getCatmullRomPoint(const Point* points, size_t index, float t, float tension);

			static float fastAtan2(const float y, const float x);
			void renderRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color);
			void renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color* color);
			void renderMissingGlyph(const Point& point, const Font* font, const Color* color, uint8_t fontScale);
			inline void renderGlyph(const Point& point, const Font* font, const Color* color, const Glyph* glyph, uint8_t fontScale);
	};
}