#pragma once

#include <cstdlib>
#include "point.h"
#include "size.h"
#include "bounds.h"
#include "fonts/font.h"
#include "color.h"
#include "hardware/screen/drivers/screenDriver.h"

namespace yoba {
	class ScreenBuffer {
		public:
			explicit ScreenBuffer(ScreenDriver* driver);

			void begin();

			virtual void allocate() = 0;
			virtual void flush() = 0;

			ScreenDriver* getDriver() const;

			Bounds& getViewport();
			void setViewport(const Bounds& bounds);
			void resetViewport();

			size_t getIndex(uint16_t x, uint16_t y) const;
			size_t getIndex(const Point& point) const;

			void clear(const Color* color) {
				clearUnchecked(color);
			}

			void renderPixel(const Point &point, const Color* color) {
				if (getViewport().intersects(point))
					renderPixelUnchecked(point, color);
			}

			void renderHorizontalLine(const Point &point, uint16_t length, const Color* color) {
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

				renderHorizontalLineUnchecked(Point(x1, point.getY()), length, color);
			}

			void renderVerticalLine(const Point &point, uint16_t length, const Color* color) {
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

				renderVerticalLineUnchecked(Point(point.getX(), y1), length, color);
			}

			void renderFilledRectangle(const Bounds& bounds, const Color* color) {
				const auto& viewport = getViewport();

				if (viewport.intersects(bounds))
					renderFilledRectangleUnchecked(viewport.getIntersection(bounds), color);
			}

			// ------------------------ Checked-by-default rendering methods ------------------------

			void renderText(const Point& point, const Font* font, const Color* color, const char* text);
			void renderText(const Point& point, const Font* font, const Color* color, const String& text);

			// Thanks, AdaFruit!
			virtual void renderLine(const Point& from, const Point& to, const Color* color);
			virtual void renderTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);
			virtual void renderFilledTriangle(const Point& point1, const Point& point2, const Point& point3, const Color* color);

		protected:
			ScreenDriver* _driver;

			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;

			Bounds _viewport = Bounds();

			virtual void clearUnchecked(const Color* color) = 0;
			virtual void renderPixelUnchecked(const Point &point, const Color* color) = 0;
			virtual void renderHorizontalLineUnchecked(const Point &point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLineUnchecked(const Point &point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangleUnchecked(const Bounds& bounds, const Color* color) = 0;
	};
}