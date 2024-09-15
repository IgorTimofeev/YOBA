#pragma once

#include <cstdlib>
#include "point.h"
#include "size.h"
#include "bounds.h"
#include "hardware/screen/drivers/screenDriver.h"
#include "fonts/font.h"
#include "color.h"

namespace yoba {
	enum class ScreenOrientation : uint8_t {
		Landscape0,
		Portrait90,
		Landscape270,
	};

	class ScreenBuffer {
		public:
			ScreenBuffer(
				ScreenDriver* driver,
				const Size& size,
				ScreenOrientation orientation
			);

			void begin();

			virtual void allocate() = 0;
			virtual void flush() = 0;

			ScreenDriver* getDriver() const;

			const Size &getSize() const;
			ScreenOrientation getRotation() const;

			Bounds& getViewport();
			void setViewport(const Bounds& bounds);
			void resetViewport();

			size_t getIndex(uint16_t x, uint16_t y) const;
			size_t getIndex(const Point& point) const;

			virtual void clear(const Color* color) = 0;
			virtual void renderPixel(const Point &point, const Color* color) = 0;
			virtual void renderHorizontalLine(const Point &point, uint16_t length, const Color* color) = 0;
			virtual void renderVerticalLine(const Point &point, uint16_t length, const Color* color) = 0;
			virtual void renderFilledRectangle(const Bounds& bounds, const Color* color) = 0;

			// ------------------------ Non-native rendering methods ------------------------
			Size getTextSize(const Font* font, const char* text);
			Size getTextSize(const Font* font, const String& text);
			
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

			const Size _size;
			ScreenOrientation _orientation;
			Bounds _viewport = Bounds();
	};
}