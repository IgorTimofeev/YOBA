#include "buffer.h"
#include "buffer.h"

namespace yoba {
	Buffer::Buffer(
		Driver* driver,
		const Size& resolution
	) :
		_driver(driver),
		_size(resolution)
	{
		resetViewport();
	}

	void Buffer::begin() {
		Serial.println("allocate()");
		allocate();

		Serial.println("_driver begin()");

		_driver->begin(this);
	}

	Driver* Buffer::getDriver() const {
		return _driver;
	}

	const Size& Buffer::getSize() const {
		return _size;
	}

	Bounds &Buffer::getViewport() {
		return _viewport;
	}

	void Buffer::setViewport(const Bounds& bounds) {
		_viewport = bounds;
	}

	void Buffer::resetViewport() {
		_viewport.setX(0);
		_viewport.setY(0);
		_viewport.setWidth(_size.getWidth());
		_viewport.setHeight(_size.getHeight());
	}

	size_t Buffer::getIndex(uint16_t x, uint16_t y) const {
		return y * getSize().getWidth() + x;
	}

	size_t Buffer::getIndex(const Point &point) const {
		return getIndex(point.getX(), point.getY());
	}
}