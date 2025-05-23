#include "renderer.h"
#include <YOBA/main/math.h>

namespace YOBA {
	RenderTarget* Renderer::getTarget() const {
		return _target;
	}

	void Renderer::setTarget(RenderTarget* value) {
		if (value == _target || (_target && *value == *_target))
			return;

		const auto previousTarget = _target;

		if (previousTarget) {
			previousTarget->_renderer = nullptr;
		}

		_target = value;
		_target->_renderer = this;

		resetViewport();

		updateFromTarget();
	}

	void Renderer::updateFromTarget() {

	}

	const Bounds& Renderer::getViewport() const {
		return _viewport;
	}

	void Renderer::setViewport(const Bounds& viewport) {
		_viewport = viewport;
	}

	Bounds Renderer::pushViewport(const Bounds& bounds) {
		const auto oldViewport = _viewport;

		if (_viewport.intersects(bounds)) {
			_viewport = _viewport.getIntersection(bounds);
		}
		else {
			_viewport = { 0, 0, 0, 0 };
		}

		return oldViewport;
	}

	void Renderer::popViewport(const Bounds& bounds) {
		_viewport = bounds;
	}

	void Renderer::resetViewport() {
		_viewport.setX(0);
		_viewport.setX(0);
		_viewport.setSize(_target->getSize());
	}

	// -------------------------------- Native rendering --------------------------------

	void Renderer::clear(const Color* color) {
		clearNative(color);
	}

	void Renderer::renderPixel(const Point& point, const Color* color) {
		if (!getViewport().intersects(point))
			return;

		renderPixelNative(point, color);
	}

	void Renderer::renderHorizontalLine(const Point& point, uint16_t length, const Color* color) {
		const auto& viewport = getViewport();

		if (
			length == 0
			|| point.getX() > viewport.getX2()
			|| point.getX() + length < viewport.getX()

			|| point.getY() < viewport.getY()
			|| point.getY() > viewport.getY2()
			)
			return;

		const uint16_t x1 = std::max(point.getX(), viewport.getX());
		const uint16_t x2 = std::min(point.getX() + length - 1, viewport.getX2());
		length = x2 - x1 + 1;

		renderHorizontalLineNative(Point(x1, point.getY()), length, color);
	}

	void Renderer::renderVerticalLine(const Point& point, uint16_t length, const Color* color) {
		const auto& viewport = getViewport();

		if (
			length == 0
			|| point.getX() < viewport.getX()
			|| point.getX() > viewport.getX2()

			|| point.getY() > viewport.getY2()
			|| point.getY() + length < viewport.getY()
		)
			return;

		const uint16_t y1 = std::max(point.getY(), viewport.getY());
		const uint16_t y2 = std::min(point.getY() + length - 1, viewport.getY2());
		length = y2 - y1 + 1;

		renderVerticalLineNative(Point(point.getX(), y1), length, color);
	}

	void Renderer::renderFilledRectangle(const Bounds& bounds, const Color* color) {
		const auto& viewport = getViewport();

		if (bounds.haveZeroSize() || !viewport.intersects(bounds))
			return;

		const auto& intersection = viewport.getIntersection(bounds);

		if (intersection.getWidth() > 1 || intersection.getHeight() > 1) {
			renderFilledRectangleNative(intersection, color);
		}
		else if (intersection.getWidth() == 1) {
			renderVerticalLine(intersection.getPosition(), intersection.getHeight(), color);
		}
		else if (intersection.getHeight() == 1) {
			renderHorizontalLine(intersection.getPosition(), intersection.getWidth(), color);
		}
		else {
			renderPixelNative(intersection.getPosition(), color);
		}
	}

	void Renderer::renderDitheredRectangle(const Bounds& bounds, const Color* color) {
		constexpr uint8_t step = 1;
		constexpr uint16_t totalSize = step + step;
		bool odd = true;

		const auto x2 = bounds.getX2();
		const auto y2 = bounds.getY2();

		for (int32_t j = bounds.getY(); j <= y2; j++) {
			for (int32_t i = odd ? bounds.getX() + step : bounds.getX(); i <= x2; i += totalSize)
				renderPixel(Point(i, j), color);

			odd = !odd;
		}
	}

	void Renderer::renderImage(const Point& point, const Image* image) {
		if (getViewport().intersects(Bounds(point, image->getSize())))
			renderImageNative(point, image);
	}

	// -------------------------------- Non-native rendering --------------------------------

	void Renderer::renderFilledRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color) {
		if (cornerRadius > 0) {
			// Rect in middle
			renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY() + cornerRadius,
					bounds.getWidth(),
					bounds.getHeight() - cornerRadius - cornerRadius
				),
				color
			);

			// 2 upper corners
			renderFilledRoundedCorners(
				Point(
					bounds.getX() + cornerRadius,
					bounds.getY() + cornerRadius
				),
				cornerRadius,
				true,
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			// And 2 lower
			renderFilledRoundedCorners(
				Point(
					bounds.getX() + cornerRadius,
					bounds.getY() + bounds.getHeight() - cornerRadius - 1
				),
				cornerRadius,
				false,
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);
		}
		else {
			renderFilledRectangle(bounds, color);
		}
	}

	void Renderer::renderRectangle(const Bounds& bounds, const Color* color) {
		if (bounds.haveZeroSize())
			return;

		if (bounds.getWidth() > 1 || bounds.getHeight() > 1) {
			renderHorizontalLine(
				bounds.getTopLeft(),
				bounds.getWidth(),
				color
			);

			renderHorizontalLine(
				bounds.getBottomLeft(),
				bounds.getWidth(),
				color
			);

			renderVerticalLine(
				Point(
					bounds.getX(),
					bounds.getY() + 1
				),
				bounds.getHeight() - 2,
				color
			);

			renderVerticalLine(
				Point(
					bounds.getX2(),
					bounds.getY() + 1
				),
				bounds.getHeight() - 2,
				color
			);
		}
		else {
			renderPixel(bounds.getPosition(), color);
		}
	}

	void Renderer::renderRectangle(const Bounds& bounds, uint16_t cornerRadius, const Color* color) {
		if (cornerRadius > 0) {
			renderHorizontalLine(
				Point(bounds.getX() + cornerRadius, bounds.getY()),
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			renderHorizontalLine(
				Point(bounds.getX() + cornerRadius, bounds.getY() + bounds.getHeight() - 1),
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			renderVerticalLine(
				Point(bounds.getX() , bounds.getY() + cornerRadius),
				bounds.getHeight() - cornerRadius - cornerRadius,
				color
			);

			renderVerticalLine(
				Point(bounds.getX() + bounds.getWidth() - 1, bounds.getY() + cornerRadius),
				bounds.getHeight() - cornerRadius - cornerRadius,
				color
			);

			renderRoundedCorners(
				Point(bounds.getX() + cornerRadius, bounds.getY() + cornerRadius),
				cornerRadius,
				1,
				color
			);

			renderRoundedCorners(
				Point(bounds.getX() + bounds.getWidth() - cornerRadius - 1, bounds.getY() + cornerRadius),
				cornerRadius,
				2,
				color
			);

			renderRoundedCorners(
				Point(bounds.getX() + bounds.getWidth() - cornerRadius - 1, bounds.getY() + bounds.getHeight() - cornerRadius - 1),
				cornerRadius,
				4,
				color
			);

			renderRoundedCorners(
				Point(bounds.getX() + cornerRadius, bounds.getY() + bounds.getHeight() - cornerRadius - 1),
				cornerRadius,
				8,
				color
			);
		}
		else {
			renderRectangle(bounds, color);
		}
	}

	void Renderer::renderLine(const Point& from, const Point& to, const Color* color) {
		// Vertical line
		if (from.getX() == to.getX()) {
			renderVerticalLine(
				Point(from.getX(), std::min(from.getY(), to.getY())),
				std::abs(to.getY() - from.getY()) + 1,
				color
			);
		}
		// Horizontal line
		else if (from.getY() == to.getY()) {
			renderHorizontalLine(
				Point(std::min(from.getX(), to.getX()), from.getY()),
				std::abs(to.getX() - from.getX()) + 1,
				color
			);
		}
		// Meh...
		else {
			const auto& viewport = getViewport();
			const auto viewportX2 = viewport.getX2();
			const auto viewportY2 = viewport.getY2();

			if (
				(from.getX() < viewport.getX() && to.getX() < viewport.getX())
				|| (from.getX() > viewportX2 && to.getX() > viewportX2)

				|| (from.getY() < viewport.getY() && to.getY() < viewport.getY())
				|| (from.getY() > viewportY2 && to.getY() > viewportY2)
			)
				return;

			int32_t
				x1 = from.getX(),
				y1 = from.getY(),

				x2 = to.getX(),
				y2 = to.getY();

			const bool deltaYGreater = std::abs(y2 - y1) > std::abs(x2 - x1);

			if (deltaYGreater) {
				std::swap(x1, y1);
				std::swap(x2, y2);
			}

			if (x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}

			const int32_t deltaX = x2 - x1;
			const int32_t deltaY = std::abs(y2 - y1);
			const int32_t yStep = y1 > y2 ? -1 : 1;
			int32_t partRemaining = deltaX >> 1;
			int32_t partVarFrom = x1;

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

	void Renderer::renderTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color) {
		// Simple as fuck
		renderLine(point1, point2, color);
		renderLine(point2, point3, color);
		renderLine(point3, point1, color);
	}

	void Renderer::renderFilledTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color) {
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

		const int32_t dx12 = x2 - x1;
		const int32_t dy12 = y2 - y1;
		const int32_t dx13 = x3 - x1;
		const int32_t dy13 = y3 - y1;
		const int32_t dx23 = x3 - x2;
		const int32_t dy23 = y3 - y2;
		int32_t sa = 0;
		int32_t sb = 0;
		int32_t last;
		int32_t y;

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

	void Renderer::renderCircle(const Point& center, uint16_t radius, const Color* color) {
		if (radius == 0)
			return;

		int32_t
			f = 1 - radius,
			ddF_y = -2 * radius,
			ddF_x = 1,
			xs = -1,
			xe = 0,
			len;

		bool first = true;

		do {
			while (f < 0) {
				++xe;
				f += (ddF_x += 2);
			}

			f += (ddF_y += 2);

			if (xe-xs > 1) {
				if (first) {
					len = 2 * (xe - xs) - 1;
					renderHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
					renderHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);
					renderVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					renderVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);
					first = false;
				}
				else {
					len = xe - xs++;
					renderHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
					renderHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);
					renderHorizontalLine(Point(center.getX() + xs, center.getY() - radius), len, color);
					renderHorizontalLine(Point(center.getX() + xs, center.getY() + radius), len, color);

					renderVerticalLine(Point(center.getX() + radius, center.getY() + xs), len, color);
					renderVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					renderVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);
					renderVerticalLine(Point(center.getX() - radius, center.getY() + xs), len, color);
				}
			}
			else {
				++xs;
				renderPixel(Point(center.getX() - xe, center.getY() + radius), color);
				renderPixel(Point(center.getX() - xe, center.getY() - radius), color);
				renderPixel(Point(center.getX() + xs, center.getY() - radius), color);
				renderPixel(Point(center.getX() + xs, center.getY() + radius), color);

				renderPixel(Point(center.getX() + radius, center.getY() + xs), color);
				renderPixel(Point(center.getX() + radius, center.getY() - xe), color);
				renderPixel(Point(center.getX() - radius, center.getY() - xe), color);
				renderPixel(Point(center.getX() - radius, center.getY() + xs), color);
			}

			xs = xe;
		}
		while (xe < --radius);
	}

	void Renderer::renderFilledCircle(const Point& center, uint16_t radius, const Color* color) {
		if (radius == 0)
			return;

		int32_t x = 0;
		int32_t dx = 1;
		int32_t dy = radius + radius;
		int32_t p = -(radius >> 1);

		renderHorizontalLine(Point(center.getX() - radius, center.getY()), dy + 1, color);

		while(x < radius){
			if (p>= 0) {
				renderHorizontalLine(Point(center.getX() - x, center.getY() + radius), dx, color);
				renderHorizontalLine(Point(center.getX() - x, center.getY() - radius), dx, color);

				dy -= 2;
				p -= dy;
				radius--;
			}

			dx += 2;
			p += dx;
			x++;

			renderHorizontalLine(Point(center.getX() - radius, center.getY() + x), dy + 1, color);
			renderHorizontalLine(Point(center.getX() - radius, center.getY() - x), dy + 1, color);
		}
	}

	void Renderer::renderArc(const Point& center, int16_t radius, int16_t start, int16_t end, const Color* color) {
		if (start == end) {
			renderCircle(center, radius, color);
			return;
		}
		
		const uint8_t inverted = (start > end);
		const uint8_t a_start = inverted ? end : start;
		const uint8_t a_end = inverted ? start : end;

		int32_t x = 0;
		int32_t y = radius;
		int32_t d = radius - 1;

		// Trace arc radius with the Andres circle algorithm (process each pixel of a 1/8th circle of radius rad)
		while (y >= x) {
			// Get the percentage of 1/8th circle drawn with a fast approximation of arctan(x/y)
			uint32_t ratio = x * 255 / y; // x/y [0..255]
			ratio = ratio * (770195 - (ratio - 255) * (ratio + 941)) / 6137491; // arctan(x/y) [0..32]

			// Fill the pixels with the 8 sections of the circle, but only on the arc defined by the angles (start and end)
			if ((ratio >= a_start && ratio < a_end) ^ inverted)
				renderPixel(Point(center.getX() + y, center.getY() - x), color);
			
			if (((ratio + a_end) > 63 && (ratio + a_start) <= 63) ^ inverted)
				renderPixel(Point(center.getX() + x, center.getY() - y), color);
			
			if (((ratio + 64) >= a_start && (ratio + 64) < a_end) ^ inverted)
				renderPixel(Point(center.getX() - x, center.getY() - y), color);
			
			if (((ratio + a_end) > 127 && (ratio + a_start) <= 127) ^ inverted)
				renderPixel(Point(center.getX() - y, center.getY() - x), color);
			
			if (((ratio + 128) >= a_start && (ratio + 128) < a_end) ^ inverted)
				renderPixel(Point(center.getX() - y, center.getY() + x), color);
			
			if (((ratio + a_end) > 191 && (ratio + a_start) <= 191) ^ inverted)
				renderPixel(Point(center.getX() - x, center.getY() + y), color);
			
			if (((ratio + 192) >= a_start && (ratio + 192) < a_end) ^ inverted)
				renderPixel(Point(center.getX() + x, center.getY() + y), color);
			
			if (((ratio + a_end) > 255 && (ratio + a_start) <= 255) ^ inverted)
				renderPixel(Point(center.getX() + y, center.getY() + x), color);
			
			// Run Andres circle algorithm to get to the next pixel
			if (d >= 2 * x) {
				d = d - 2 * x - 1;
				x = x + 1;
			}
			else if (d < 2 * (radius - y)) {
				d = d + 2 * y - 1;
				y = y - 1;
			}
			else {
				d = d + 2 * (y - x - 1);
				y = y - 1;
				x = x + 1;
			}
		}
	}

	void Renderer::renderRoundedCorners(const Point& center, int32_t radius, uint8_t corner, const Color* color) {
		int32_t f     = 1 - radius;
		int32_t ddF_x = 1;
		int32_t ddF_y = -2 * radius;
		int32_t xe    = 0;
		int32_t xs    = 0;
		int32_t len   = 0;

		do
		{
			while (f < 0) {
				++xe;
				f += (ddF_x += 2);
			}
			f += (ddF_y += 2);

			if (xe-xs==1) {
				if (corner & 0x1) { // left top
					renderPixel(Point(center.getX() - xe, center.getY() - radius), color);
					renderPixel(Point(center.getX() - radius, center.getY() - xe), color);
				}

				if (corner & 0x2) { // right top
					renderPixel(Point(center.getX() + radius    , center.getY() - xe), color);
					renderPixel(Point(center.getX() + xs + 1, center.getY() - radius), color);
				}

				if (corner & 0x4) { // right bottom
					renderPixel(Point(center.getX() + xs + 1, center.getY() + radius), color);
					renderPixel(Point(center.getX() + radius, center.getY() + xs + 1), color);
				}

				if (corner & 0x8) { // left bottom
					renderPixel(Point(center.getX() - radius, center.getY() + xs + 1), color);
					renderPixel(Point(center.getX() - xe, center.getY() + radius) , color);
				}
			}
			else {
				len = xe - xs++;
				
				if (corner & 0x1) { // left top
					renderHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);
					renderVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);
				}

				if (corner & 0x2) { // right top
					renderVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					renderHorizontalLine(Point(center.getX() + xs, center.getY() - radius), len, color);
				}

				if (corner & 0x4) { // right bottom
					renderHorizontalLine(Point(center.getX() + xs, center.getY() + radius), len, color);
					renderVerticalLine(Point(center.getX() + radius, center.getY() + xs), len, color);
				}

				if (corner & 0x8) { // left bottom
					renderVerticalLine(Point(center.getX() - radius, center.getY() + xs), len, color);
					renderHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
				}
			}
			xs = xe;
		} while (xe < radius--);
	}

	void Renderer::renderFilledRoundedCorners(const Point& center, uint16_t radius, bool upper, int32_t delta, const Color* color) {
		int32_t f = 1 - radius;
		int32_t ddF_x = 1;
		int32_t ddF_y = -radius - radius;
		int32_t y = 0;
		int32_t lineLength;

		while (y < radius) {
			if (f >= 0) {
				lineLength = y + y + delta;

				if (lineLength > 0) {
					renderHorizontalLine(Point(center.getX() - y, upper ? center.getY() - radius : center.getY() + radius), lineLength, color);
				}

				radius--;
				ddF_y += 2;
				f += ddF_y;
			}

			y++;
			ddF_x += 2;
			f += ddF_x;
			lineLength = radius + radius + delta;

			if (lineLength > 0) {
				renderHorizontalLine(Point(center.getX() - radius, upper ? center.getY() - y : center.getY() + y), lineLength, color);
			}
		}
	}

	void Renderer::renderMissingGlyph(const Point& point, const Font* font, const Color* color, uint8_t fontScale) {
		renderRectangle(
			Bounds(
				point.getX(),
				point.getY(),
				Font::missingGlyphWidth * fontScale,
				font->getHeight() * fontScale
			),
			color
		);
	}

	void Renderer::renderGlyph(const Point& point, const Font* font, const Color* color, const Glyph* glyph, uint8_t fontScale) {
		auto bitIndex = glyph->getBitmapIndex();
		uint8_t bitmapByte;

		// Non-scaled fonts can be drawn a little bit faster
		if (fontScale == 1) {
			for (uint8_t j = 0; j < font->getHeight(); j++) {
				for (uint8_t i = 0; i < font->getWidth(glyph); i++) {
					bitmapByte = font->getBitmap()[bitIndex / 8];

					if ((bitmapByte >> bitIndex % 8) & 1)
						renderPixel(Point(point.getX() + i, point.getY() + j), color);

					bitIndex++;
				}
			}
		}
		else {
			int32_t
				x = point.getX(),
				y = point.getY();

			for (uint8_t j = 0; j < font->getHeight(); j++) {
				for (uint8_t i = 0; i < font->getWidth(glyph); i++) {
					bitmapByte = font->getBitmap()[bitIndex / 8];

					if ((bitmapByte >> bitIndex % 8) & 1)
						renderFilledRectangle(Bounds(x, y, fontScale, fontScale), color);

					bitIndex++;
					x += fontScale;
				}

				x = point.getX();
				y += fontScale;
			}
		}
	}

	void Renderer::renderString(const Point& point, const Font* font, const Color* color, std::wstring_view string, uint8_t fontScale) {
		const auto& viewport = getViewport();
		const auto viewportX2 = viewport.getX2();

		// Skipping rendering if text is obviously not in viewport
		if (
			point.getX() > viewportX2
			|| point.getY() > viewport.getY2()
			|| point.getY() + font->getHeight(fontScale) < viewport.getY()
			|| !color
		)
			return;

		int32_t x = point.getX();

		for (size_t charIndex = 0; charIndex < string.length(); charIndex++) {
			// Trying to find glyph matched to char
			const auto glyph = font->getGlyph(string[charIndex]);

			// If glyph was found in bitmap & can be rendered as "human-readable"
			// For example,U+007F "DEL" symbol often has zero width in some fonts
			if (glyph && font->getWidth(glyph) > 0) {
				const int32_t x2 = x + font->getWidth(glyph, fontScale);

				// Rendering current glyph only if it's in viewport
				if (x2 > viewport.getX()) {
					renderGlyph(
						Point(
							x,
							point.getY()
						),
						font,
						color,
						glyph,
						fontScale
					);
				}

				x = x2;
			}
			else {
				renderMissingGlyph(Point(x, point.getY()), font, color, fontScale);

				x += Font::missingGlyphWidth * fontScale;
			}

			// Stopping rendering if next glyphs will not be in viewport
			if (x > viewportX2)
				break;
		}
	}

	void Renderer::renderChar(const Point& point, const Font* font, const Color* color, wchar_t ch, uint8_t fontScale) {
		const auto& viewport = getViewport();
		const auto viewportX2 = viewport.getX2();

		if (
			point.getX() > viewportX2
			|| point.getY() > viewport.getY2()
			|| point.getY() + font->getHeight(fontScale) < viewport.getY()
			|| !color
		)
			return;

		const auto glyph = font->getGlyph(ch);

		if (glyph) {
			if (point.getX() + font->getWidth(glyph, fontScale) < viewport.getX())
				return;

			renderGlyph(
				point,
				font,
				color,
				glyph,
				fontScale
			);
		}
		else {
			renderMissingGlyph(point, font, color, fontScale);
		}
	}
}