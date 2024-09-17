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

			// -------------------------------- Rendering --------------------------------

			void clear(const Color *color);

			void renderPixel(const Point &point, const Color *color);

			void renderHorizontalLine(const Point &point, uint16_t length, const Color *color);

			void renderVerticalLine(const Point &point, uint16_t length, const Color *color);

			void renderFilledRectangle(const Bounds &bounds, const Color *color);

			void renderFilledRectangle(const Bounds &bounds, uint16_t radius, const Color* color);

			void renderString(const Point &point, const Font *font, const Color *color, const char *text);

			void renderString(const Point &point, const Font *font, const Color *color, const String &text);

			// Thanks, AdaFruit!
			void renderLine(const Point &from, const Point &to, const Color *color);

			void renderTriangle(const Point &point1, const Point &point2, const Point &point3, const Color *color);

			void renderFilledTriangle(const Point &point1, const Point &point2, const Point &point3, const Color *color);

			void renderFilledCircle(const Point &center, uint16_t radius, const Color *color);

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

		private:
			void renderRoundedCorners(const Point& center, int32_t radius, uint8_t corners, int32_t delta, const Color *color);
	};
}