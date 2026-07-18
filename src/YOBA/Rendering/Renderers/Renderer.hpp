#pragma once

#include <string>

#include <YOBA/Rendering/Targets/RenderingTarget.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Core/Image.hpp>
#include <YOBA/Core/Font.hpp>

namespace YOBA {
	class Renderer {
		friend class RenderingTarget;

		public:
			virtual ~Renderer() = default;

			RenderingTarget* getTarget() const;
			void setTarget(RenderingTarget* value);

			const Rectangle& getClip() const;
			void setClip(const Rectangle& bounds);
			Rectangle pushClip(const Rectangle& bounds);
			void resetClip();

			void clear(const Color* color);
			void putPixel(const Point& position, const Color* color);
			void strokeHorizontalLine(const Point& position, uint16_t length, const Color* color);
			void strokeVerticalLine(const Point& position, uint16_t length, const Color* color);
			void fillRectangle(const Rectangle& bounds, const Color* color);
			void fillRectangle(const Rectangle& bounds, uint16_t cornerRadius, const Color* color);
			void fillChessPatternRectangle(const Rectangle& bounds, const Color* color, uint8_t filledStep = 1, uint8_t transparentStep = 1);
			void fillQuad(const Point& topLeft, const Point& topRight, const Point& bottomRight, const Point& bottomLeft, const Color* color);

			void putImage(const Point& position, const Image* image);
			void strokeRectangle(const Rectangle& bounds, const Color* color);
			void strokeRectangle(const Rectangle& bounds, uint16_t cornerRadius, const Color* color);
			// Thanks, AdaFruit!
			void strokeLine(const Point& from, const Point& to, const Color* color);
			void strokeLine(const Point& from, const Point& to, const Color* color, uint8_t thickness);
			void strokeTriangle(const Point& position1, const Point& position2, const Point& position3, const Color* color);
			void strokeCircle(const Point& center, const uint16_t radius, const Color* color);
			void strokeCircle(const Point& center, const uint16_t outerRadius, const uint16_t thickness, const Color* color);

			void fillTriangle(const Point& position1, const Point& position2, const Point& position3, const Color* color);
			void fillCircle(const Point& center, uint16_t radius, const Color* color);

			/// @brief Strokes arc counter-clockwise, started from <b>x = 1, y = 0</b>
			/// @code
			///        y
			///        | <- )
			///        |      )
			/// -------+------ * x
			///        |
			///        |
			/// @endcode
			void strokeArc(const Point& center, uint16_t radius, float fromAngleRad, float arcMeasureRad, const Color* color);
			void strokeArc(const Point& center, uint16_t outerRadius, uint16_t thickness, float fromAngleRad, float arcMeasureRad, const Color* color);
			void strokeCatmullRomSpline(const Point* points, const size_t pointsLength, const Color* color, const uint16_t segmentsPerCurve = 10, const float tension = 0.5f);

			/**
			* @brief Render single char
			* @param position Position of upper-left corner of the char
			* @param font Font with which the char will be rendered
			* @param color Color with which the char will be rendered
			* @param codepoint UTF-8 codepoint of char
			*/
			void putText(const Point& position, const Font* font, const Color* color, uint32_t codepoint);

			/**
			* @brief Render single char
			* @param position Position of upper-left corner of the char
			* @param font Font with which the char will be rendered
			* @param fontScale Scale factor of font
			* @param color Color with which the char will be rendered
			* @param codepoint UTF-8 codepoint of char
			*/
			void putText(const Point& position, const Font* font, const uint8_t fontScale, const Color* color, uint32_t codepoint);

			/**
			* @brief Render line of UTF-8 text
			* @param position Position of upper-left corner of the first character in text
			* @param font Font with which the text will be rendered
			* @param color Color with which the text will be rendered
			* @param text Text to render
			*/
			void putText(const Point& position, const Font* font, const Color* color, std::string_view text);

			/**
			* @brief Render line of UTF-8 text
			* @param position Position of upper-left corner of the first character in text
			* @param font Font with which the text will be rendered
			* @param fontScale Scale factor of font
			* @param color Color with which the text will be rendered
			* @param text Text to render
			*/
			void putText(const Point& position, const Font* font, const uint8_t fontScale, const Color* color, std::string_view text);

			/**
			* @brief Sends pixel data to assigned <b>RenderingTarget</b>
			*/
			virtual void flush() = 0;

		protected:
			RenderingTarget* _target = nullptr;
			Rectangle _clip {};

			virtual void updateFromTarget();

			virtual void clearNative(const Color* color) = 0;
			virtual void putPixelNative(const Point& position, const Color* color) = 0;
			virtual void strokeHorizontalLineNative(const Point& position, uint16_t length, const Color* color) = 0;
			virtual void strokeVerticalLineNative(const Point& position, uint16_t length, const Color* color) = 0;
			virtual void fillRectangleNative(const Rectangle& bounds, const Color* color) = 0;
			virtual void putImageNative(const Point& position, const Image* image) = 0;

		private:
			static float getAtan2Fast(const float y, const float x);
			static Point getCatmullRomPoint(const Point* points, size_t index, float t, float tension);

			void strokeRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color);
			void fillUpperRoundedCorners(const Point& center, uint16_t radius, int32_t delta, const Color* color);
			void fillLowerRoundedCorners(const Point& center, uint16_t radius, int32_t delta, const Color* color);
			void putMissingGlyph(const Point& position, const Font* font, uint8_t fontScale, const Color* color);
			void putGlyph(const Point& position, const Font* font, uint8_t fontScale, const Color* color, int32_t glyphIndex, const Glyph* glyph);
	};
}