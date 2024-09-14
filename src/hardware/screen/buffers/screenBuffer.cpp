#include "screenBuffer.h"
#include "screenBuffer.h"

namespace yoba {
	ScreenBuffer::ScreenBuffer(
		ScreenDriver* driver,
		const Size& resolution
	) :
		_driver(driver),
		_size(resolution)
	{
		resetViewport();
	}

	void ScreenBuffer::begin() {
		Serial.println("allocate()");
		allocate();

		Serial.println("_driver begin()");

		_driver->begin(this);
	}

	ScreenDriver* ScreenBuffer::getDriver() const {
		return _driver;
	}

	const Size& ScreenBuffer::getSize() const {
		return _size;
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
		_viewport.setWidth(_size.getWidth());
		_viewport.setHeight(_size.getHeight());
	}

	size_t ScreenBuffer::getIndex(uint16_t x, uint16_t y) const {
		return y * getSize().getWidth() + x;
	}

	size_t ScreenBuffer::getIndex(const Point &point) const {
		return getIndex(point.getX(), point.getY());
	}
}