#include "screenBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	ScreenBuffer::ScreenBuffer(ScreenDriver* driver) : _driver(driver) {
		resetViewport();
	}

	void ScreenBuffer::begin() {
		allocate();

		_driver->begin();
	}

	ScreenDriver* ScreenBuffer::getDriver() const {
		return _driver;
	}

	Bounds &ScreenBuffer::getViewport() {
		return _viewport;
	}

	void ScreenBuffer::setViewport(const Bounds& bounds) {
		_viewport = bounds;
	}

	void ScreenBuffer::resetViewport() {
		_viewport.setX(0);
		_viewport.setY(0);
		_viewport.setWidth(_driver->getSize().getWidth());
		_viewport.setHeight(_driver->getSize().getHeight());
	}

	size_t ScreenBuffer::getIndex(uint16_t x, uint16_t y) const {
		return y * _driver->getSize().getWidth() + x;
	}

	size_t ScreenBuffer::getIndex(const Point &point) const {
		return getIndex(point.getX(), point.getY());
	}

	void ScreenBuffer::renderLine(const Point &from, const Point &to, const Color* color) {
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

	void ScreenBuffer::renderTriangle(const Point &point1, const Point &point2, const Point &point3, const Color* color) {
		// Simple as fuck
		renderLine(point1, point2, color);
		renderLine(point2, point3, color);
		renderLine(point3, point1, color);
	}

	void ScreenBuffer::renderFilledTriangle(const Point &point1, const Point &point2, const Point &point3, const Color* color) {
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

	void ScreenBuffer::renderText(const Point &point, const Font* font, const Color* color, const char* text) {
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

	void ScreenBuffer::renderText(const Point &point, const Font *font, const Color *color, const String &text) {
		renderText(point, font, color, text.c_str());
	}
}