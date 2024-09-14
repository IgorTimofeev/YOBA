#pragma once

#include "screenBuffer.h"
#include "fonts/font.h"

namespace yoba {
	template<typename TColor>
	class RenderBuffer : public ScreenBuffer {
		public:
			RenderBuffer(ScreenDriver *driver, const Size &resolution);

			void clear(TColor color);
			void renderPixel(const Point &point, TColor color);
			void renderHorizontalLine(const Point &point, uint16_t length, TColor color);
			void renderVerticalLine(const Point &point, uint16_t length, TColor color);
			void renderFilledRectangle(const Bounds& bounds, TColor color);

			// Thanks, AdaFruit!
			void renderLine(const Point& from, const Point& to, TColor color);
			// Thanks, AdaFruit!
			void renderTriangle(const Point& point1, const Point& point2, const Point& point3, TColor color);
			// Thanks, AdaFruit!
			void renderFilledTriangle(const Point& point1, const Point& point2, const Point& point3, TColor color);

			void renderText(const Point& point, Font* font, TColor color, const char* text);
			Size getTextSize(Font* font, const char* text);

		protected:
			virtual void clearNative(TColor color) = 0;
			virtual void renderPixelNative(const Point &point, TColor color) = 0;
			virtual void renderVerticalLineNative(const Point &point, uint16_t width, TColor color) = 0;
			virtual void renderHorizontalLineNative(const Point &point, uint16_t width, TColor color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, TColor color) = 0;
	};

	template<typename TColor>
	RenderBuffer<TColor>::RenderBuffer(ScreenDriver *driver, const Size &resolution) : ScreenBuffer(driver, resolution) {

	}

	template<typename TColor>
	void RenderBuffer<TColor>::clear(TColor color) {
		clearNative(color);
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderPixel(const Point &point, TColor color) {
		if (getViewport().intersects(point))
			renderPixelNative(point, color);
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderHorizontalLine(const Point &point, uint16_t length, TColor color) {
		const auto& viewport = getViewport();

		if (
			point.getX() > viewport.getX2()
			|| point.getX() + length < viewport.getX()

			|| point.getY() < viewport.getY()
			|| point.getY() > viewport.getY2()
		)
			return;

		uint16_t x1 = max(point.getX(), viewport.getX());
		uint16_t x2 = min(point.getX() + length - 1, viewport.getX2());
		length = x2 - x1 + 1;

		renderHorizontalLineNative(Point(x1, point.getY()), length, color);
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderVerticalLine(const Point &point, uint16_t length, TColor color) {
		const auto& viewport = getViewport();

		if (
			point.getX() < viewport.getX()
			|| point.getX() > viewport.getX2()

			|| point.getY() > viewport.getY2()
			|| point.getY() + length < viewport.getY()
		)
			return;

		uint16_t y1 = max(point.getY(), viewport.getY());
		uint16_t y2 = min(point.getY() + length - 1, viewport.getY2());
		length = y2 - y1 + 1;

		renderVerticalLineNative(Point(point.getX(), y1), length, color);
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderLine(const Point &from, const Point &to, TColor color) {
		// Vertical line
		if (from.getX() == to.getX()) {
			renderVerticalLine(from, to.getY() - from.getY() + 1, color);
		}
		// Horizontal line
		else if (from.getY() == to.getY()) {
			renderHorizontalLine(from, to.getX() - from.getX() + 1, color);
		}
		// Meh...
		else {
			int32_t
				x1 = from.getX(),
				y1 = from.getY(),

				x2 = to.getX(),
				y2 = to.getY();

			bool deltaYGreater = abs(y2 - y1) > abs(x2 - x1);

			if (deltaYGreater) {
				std::swap(x1, y1);
				std::swap(x2, y2);
			}

			if (x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}

			int32_t
				deltaX = x2 - x1,
				deltaY = abs(y2 - y1),

				partRemaining = deltaX >> 1,
				yStep = y1 > y2 ? -1 : 1,
				partVarFrom = x1;

			uint16_t partLength = 0;

			if (deltaYGreater) {
				while (x1 <= x2) {
					partLength++;
					partRemaining -= deltaY;

					if (partRemaining < 0) {
						partRemaining += deltaX;

						if (partLength == 1) {
							renderPixel(Point(y1, partVarFrom), color);
						}
						else {
							renderVerticalLine(Point(y1, partVarFrom), partLength, color);
						}

						partLength = 0;
						partVarFrom = x1 + 1;
						y1 += yStep;
					}

					x1++;
				}

				if (partLength > 0)
					renderVerticalLine(Point(y1, partVarFrom), partLength, color);
			}
			else {
				while (x1 <= x2) {
					partLength++;
					partRemaining -= deltaY;

					if (partRemaining < 0) {
						partRemaining += deltaX;

						if (partLength == 1) {
							renderPixel(Point(partVarFrom, y1), color);
						}
						else {
							renderHorizontalLine(Point(partVarFrom, y1), partLength, color);
						}

						partLength = 0;
						partVarFrom = x1 + 1;
						y1 += yStep;
					}

					x1++;
				}

				if (partLength > 0)
					renderHorizontalLine(Point(partVarFrom, y1), partLength, color);
			}
		}
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderTriangle(const Point &point1, const Point &point2, const Point &point3, TColor color) {
		// Simple as fuck
		renderLine(point1, point2, color);
		renderLine(point2, point3, color);
		renderLine(point3, point1, color);
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderFilledTriangle(const Point &point1, const Point &point2, const Point &point3, TColor color) {
		int32_t
			x1 = point1.getX(),
			y1 = point1.getY(),
			x2 = point2.getX(),
			y2 = point2.getY(),
			x3 = point3.getX(),
			y3 = point3.getY();

		// Sort coordinates by Y order (y2 >= y1 >= y0)
		if (y1 > y2) {
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		if (y2 > y3) {
			std::swap(y3, y2);
			std::swap(x3, x2);
		}

		if (y1 > y2) {
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		int32_t a, b;

		// No triangle here, just single line
		if (y1 == y3) {
			a = b = x1;

			if (x2 < a) {
				a = x2;
			}
			else if (x2 > b) {
				b = x2;
			}

			if (x3 < a) {
				a = x3;
			}
			else if (x3 > b) {
				b = x3;
			}

			renderHorizontalLine(Point(a, y1), b - a + 1, color);

			return;
		}

		int32_t
			dx12 = x2 - x1,
			dy12 = y2 - y1,
			dx13 = x3 - x1,
			dy13 = y3 - y1,
			dx23 = x3 - x2,
			dy23 = y3 - y2,
			sa = 0,
			sb = 0,
			last,
			y;

		// For upper part of triangle, find scanline crossings for segments
		// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
		// is included here (and second loop will be skipped, avoiding a /0
		// error there), otherwise scanline y1 is skipped here and handled
		// in the second loop...which also avoids a /0 error here if y0=y1
		// (flat-topped triangle).
		if (y2 == y3) {
			last = y2;  // Include y1 scanline
		}
		else {
			last = y2 - 1; // Skip it
		}

		for (y = y1; y <= last; y++) {
			a = x1 + sa / dy12;
			b = x1 + sb / dy13;
			sa += dx12;
			sb += dx13;

			if (a > b)
				std::swap(a, b);

			renderHorizontalLine(Point(a, y), b - a + 1, color);
		}

		// For lower part of triangle, find scanline crossings for segments
		// 0-2 and 1-2.  This loop is skipped if y1=y2.
		sa = dx23 * (y - y2);
		sb = dx13 * (y - y1);

		while (y <= y3) {
			a = x2 + sa / dy23;
			b = x1 + sb / dy13;
			sa += dx23;
			sb += dx13;

			if (a > b)
				std::swap(a, b);

			renderHorizontalLine(Point(a, y), b - a + 1, color);

			y++;
		}
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderFilledRectangle(const Bounds &bounds, TColor color) {
		const auto& viewport = getViewport();

		if (viewport.intersects(bounds))
			renderFilledRectangleNative(viewport.getIntersection(bounds), color);
	}

	template<typename TColor>
	Size RenderBuffer<TColor>::getTextSize(Font *font, const char *text) {
		const char* charPtr;
		size_t charIndex = 0;
		const Glyph* glyph;

		uint16_t width = 0;

		while (true) {
			charPtr = text + charIndex;

			// End of text
			if (*charPtr == '\0')
				break;

			// Trying to find glyph matched to char
			glyph = font->getGlyph(*charPtr);

			if (glyph) {
				width += glyph->getWidth();
			}
			// For non-existing glyphs we can just simulate whitespace
			else {
				width += 10;
			}

			charIndex++;
		}

		return {
			width,
			font->getHeight()
		};
	}

	template<typename TColor>
	void RenderBuffer<TColor>::renderText(const Point &point, Font* font, TColor color, const char* text) {
		const char* charPtr;
		size_t charIndex = 0;
		const Glyph* glyph;

		int32_t x = point.getX();

		uint32_t bitmapBitIndex;
		uint8_t bitmapByte;

		while (true) {
			charPtr = text + charIndex;

			// End of text
			if (*charPtr == '\0')
				break;

			// Trying to find glyph matched to char
			glyph = font->getGlyph(*charPtr);

			if (glyph) {
				bitmapBitIndex = glyph->getBitmapBitIndex();

				for (int j = 0; j < font->getHeight(); j++) {
					for (int i = 0; i < glyph->getWidth(); i++) {
						bitmapByte = font->getBitmap()[bitmapBitIndex / 8];

						// We have pixel!
						if ((bitmapByte >> bitmapBitIndex % 8) & 0b1)
							renderPixel(Point(x + i, point.getY() + j), color);

						bitmapBitIndex++;
					}
				}

				x += glyph->getWidth();
			}
			// For non-existing glyphs we can just simulate whitespace
			else {
				x += 10;
			}

			charIndex++;
		}
	}
}