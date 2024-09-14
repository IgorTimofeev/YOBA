#pragma once

#include <cstdlib>
#include "point.h"
#include "size.h"
#include "bounds.h"
#include "hardware/screen/drivers/screenDriver.h"

namespace yoba {
	class ScreenBuffer {
		public:
			ScreenBuffer(
				ScreenDriver* driver,
				const Size& resolution
			);

			void begin();

			ScreenDriver* getDriver() const;

			const Size &getSize() const;

			Bounds& getViewport();

			void setViewport(const Bounds& bounds);

			void resetViewport();

			virtual void allocate() = 0;
			virtual void flush() = 0;

			size_t getIndex(uint16_t x, uint16_t y) const;
			size_t getIndex(const Point& point) const;

		protected:
			ScreenDriver* _driver;

			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;

			const Size _size;
			Bounds _viewport = Bounds();
	};
}