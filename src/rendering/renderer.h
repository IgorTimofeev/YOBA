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

			const Size& getSize() const;

			const Bounds& getViewport();
			void setViewport(const Bounds& bounds);
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
			* @param text Template-based pointer to first character in text
			*/
			template<typename TChar>
			void renderText(const Point& point, const Font* font, const Color* color, const std::basic_string_view<TChar>& text);
			void renderText(const Point& point, const Font* font, const Color* color, const std::string_view& text);
			void renderText(const Point& point, const Font* font, const Color* color, const std::wstring_view& text);

			template<typename TChar>
			void renderChar(const Point& point, const Font* font, const Color* color, TChar ch);
			void renderChar(const Point& point, const Font* font, const Color* color, char ch);
			void renderChar(const Point& point, const Font* font, const Color* color, wchar_t ch);

			virtual void flush() = 0;

			// -------------------------------- Default colors --------------------------------

			const Color* getPrimaryColor() const;
			void setPrimaryColor(const Color* value);

			const Color* getSecondaryColor() const;
			void setSecondaryColor(const Color* value);

		protected:
			RenderTarget* _renderTarget;

			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;

			Bounds _viewport = Bounds();

			virtual void clearNative(const Color* color) = 0;
			virtual void renderPixelNative(const Point& point, const Color* color) = 0;
			virtual void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, const Color* color) = 0;
			virtual void renderImageNative(const Point& point, const Image* image) = 0;

		private:
			const Color* _primaryColor = nullptr;
			const Color* _secondaryColor = nullptr;

			void renderRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color);
			void renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color* color);
			inline void renderGlyph(const Point& point, const Font* font, const Color* color, const Glyph* glyph);

			void renderMissingGlyph(const Point& point, const Font* font, const Color* color);
	};

	template<typename TChar>
	void Renderer::renderText(const Point& point, const Font* font, const Color* color, const std::basic_string_view<TChar>& text) {
		const auto& viewport = getViewport();
		const auto viewportX2 = viewport.getX2();

		// Skipping rendering if text is obviously not in viewport
		if (
			point.getX() > viewportX2
			|| point.getY() > viewport.getY2()
			|| point.getY() + font->getHeight() < viewport.getY()
		)
			return;

		const Glyph* glyph;

		int32_t
			x = point.getX(),
			x2;

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++) {
			// Trying to find glyph matched to char
			glyph = font->getGlyph(text[charIndex]);

			// If glyph was found in bitmap & can be rendered as "human-readable"
			// For example,U+007F "DEL" symbol often has zero width in some fonts
			if (glyph && glyph->getWidth() > 0) {
				x2 = x + glyph->getWidth();

				// Rendering current glyph only if it's in viewport
				if (x2 > viewport.getX()) {
					renderGlyph(
						Point(
							x,
							point.getY()
						),
						font,
						color,
						glyph
					);
				}

				x = x2;
			}
			else {
				renderMissingGlyph(Point(x, point.getY()), font, color);

				x += Font::missingGlyphWidth;
			}

			// Stopping rendering if next glyphs will not be in viewport
			if (x > viewportX2)
				break;
		}
	}

	template<typename TChar>
	void Renderer::renderChar(const Point& point, const Font* font, const Color* color, TChar ch) {
		const auto& viewport = getViewport();
		const auto viewportX2 = viewport.getX2();

		if (
			point.getX() > viewportX2
			|| point.getY() > viewport.getY2()
			|| point.getY() + font->getHeight() < viewport.getY()
		)
			return;

		const auto glyph = font->getGlyph(ch);

		if (glyph) {
			if (point.getX() + glyph->getWidth() < viewport.getX())
				return;

			renderGlyph(
				point,
				font,
				color,
				glyph
			);
		}
		else {
			renderMissingGlyph(point, font, color);
		}
	}
}