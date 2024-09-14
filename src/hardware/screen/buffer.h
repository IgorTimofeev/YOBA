#pragma once

#include <cstdlib>
#include "yoba/point.h"
#include "yoba/size.h"
#include "yoba/bounds.h"
#include "yoba/hardware/screen/drivers/driver.h"

class Buffer {
	public:
		Buffer(
			Driver* driver,
			const Size& resolution
		);

		void begin();

		Driver* getDriver() const;

		const Size &getSize() const;

		Bounds& getViewport();

		void setViewport(const Bounds& bounds);

		void resetViewport();

		virtual void allocate() = 0;
		virtual void flush() = 0;

		size_t getIndex(uint16_t x, uint16_t y) const;
		size_t getIndex(const Point& point) const;

	protected:
		Driver* _driver;

		uint8_t* _buffer = nullptr;
		size_t _bufferLength = 0;

		const Size _size;
		Bounds _viewport = Bounds();
};