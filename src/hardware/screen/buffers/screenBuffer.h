#pragma once

#include <cstdlib>
#include "../../../point.h"
#include "../../../size.h"
#include "../../../bounds.h"
#include "../../../color.h"
#include "../../../font.h"
#include "../../../image.h"
#include "../drivers/screenDriver.h"

namespace yoba {
	class ScreenBuffer {
		public:
			explicit ScreenBuffer(ScreenDriver *driver);

			void begin();

			ScreenDriver *getDriver() const;

			virtual void allocate() = 0;

			virtual void flush() = 0;

			Bounds &getViewport();

			void setViewport(const Bounds &bounds);

			void resetViewport();

			size_t getIndex(uint16_t x, uint16_t y) const;

			size_t getIndex(const Point &point) const;

			// -------------------------------- Native rendering --------------------------------

			void clear(const Color *color);

			void renderPixel(const Point &point, const Color *color);

			void renderHorizontalLine(const Point &point, uint16_t length, const Color *color);

			void renderVerticalLine(const Point &point, uint16_t length, const Color *color);

			void renderFilledRectangle(const Bounds &bounds, const Color *color);

			void renderFilledRectangle(const Bounds &bounds, uint16_t radius, const Color* color);

			void renderImage(const Point& point, const Image* image);

			// -------------------------------- Non-native rendering --------------------------------

			// Thanks, AdaFruit!
			void renderLine(const Point &from, const Point &to, const Color *color);

			void renderTriangle(const Point &point1, const Point &point2, const Point &point3, const Color *color);

			void renderFilledTriangle(const Point &point1, const Point &point2, const Point &point3, const Color *color);

			void renderFilledCircle(const Point &center, uint16_t radius, const Color *color);

			template<typename TChar>
			void renderText(const Point &point, const Font *font, const Color *color, const TChar* text);

		protected:
			ScreenDriver *_driver;

			uint8_t *_buffer = nullptr;
			size_t _bufferLength = 0;

			Bounds _viewport = Bounds();

			virtual void clearNative(const Color *color) = 0;

			virtual void renderPixelNative(const Point &point, const Color *color) = 0;

			virtual void renderHorizontalLineNative(const Point &point, uint16_t length, const Color *color) = 0;

			virtual void renderVerticalLineNative(const Point &point, uint16_t length, const Color *color) = 0;

			virtual void renderFilledRectangleNative(const Bounds &bounds, const Color *color) = 0;

			virtual void renderImageNative(const Point& point, const Image* image) = 0;

		private:
			void renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color *color);
	};

	template<typename TChar>
	void ScreenBuffer::renderText(const Point& point, const Font* font, const Color* color, const TChar* text) {
		const auto viewportX2 = getViewport().getX2();

		// Skipping rendering if text is obviously not in viewport
		if (
			point.getX() > viewportX2
			|| point.getY() > getViewport().getY2()
			|| point.getY() + font->getHeight() < getViewport().getY()
			)
			return;

		TChar ch;
		size_t charIndex = 0;
		const Glyph* glyph;

		int32_t
			x = point.getX(),
			x2;

		uint32_t bitmapBitIndex;
		uint8_t bitmapByte;

		while (true) {
			ch = text[charIndex];

			// End of text
			if (ch == '\0')
				break;

			// Trying to find glyph matched to char
			glyph = font->getGlyph(ch);

			// If glyph was found in bitmap & can be rendered as "human-readable"
			// For example,U+007F "DEL" symbol often has zero width in some fonts
			if (glyph && glyph->getWidth() > 0) {
				x2 = x + glyph->getWidth();

				// Rendering current glyph only if it's in viewport
				if (x2 > getViewport().getX()) {
					bitmapBitIndex = glyph->getBitmapBitIndex();

					for (uint8_t j = 0; j < font->getHeight(); j++) {
						for (uint8_t i = 0; i < glyph->getWidth(); i++) {
							bitmapByte = font->getBitmap()[bitmapBitIndex / 8];

							// We have pixel!
							if ((bitmapByte >> bitmapBitIndex % 8) & 1)
								renderPixel(Point(x + i, point.getY() + j), color);

							bitmapBitIndex++;
						}
					}
				}

				x = x2;

				// Stopping rendering if next glyphs will not be in viewport
				if (x > viewportX2)
					break;
			}
				// For non-existing glyphs we can just simulate whitespace
			else {
				x += Font::missingGlyphWidth;
			}

			charIndex++;
		}
	}
}