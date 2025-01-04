#include "bounds.h"

namespace yoba {
	Bounds::Bounds(int32_t x, int32_t y, int32_t width, int32_t height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{

	}

	Bounds::Bounds(const Bounds& bounds) : Bounds(bounds._x, bounds._y, bounds._width, bounds._height) {

	}

	Bounds::Bounds() : Bounds(0, 0, 0, 0) {

	}

	Bounds::Bounds(const Point& position, const Size& size) : Bounds(position.getX(), position.getY(), size.getWidth(), size.getHeight()) {

	}

	Bounds::Bounds(const Size& size) : Bounds(Point(), size) {

	}

	int32_t Bounds::getX() const {
		return _x;
	}

	void Bounds::setX(int32_t value) {
		_x = value;
	}

	int32_t Bounds::getY() const {
		return _y;
	}

	void Bounds::setY(int32_t value) {
		_y = value;
	}

	uint16_t Bounds::getWidth() const {
		return _width;
	}

	void Bounds::setWidth(uint16_t value) {
		_width = value;
	}

	uint16_t Bounds::getHeight() const {
		return _height;
	}

	void Bounds::setHeight(uint16_t value) {
		_height = value;
	}

	Point Bounds::getSize() const {
		return {getWidth(), getHeight()};
	}

	int32_t Bounds::getX2() const {
		return _x + _width - 1;
	}

	int32_t Bounds::getY2() const {
		return _y + _height - 1;
	}

	int32_t Bounds::getXCenter() const {
		return _x + _width / 2;
	}

	int32_t Bounds::getYCenter() const {
		return _y + _height / 2;
	}

	Point Bounds::getCenter() const {
		return {
			getXCenter(),
			getYCenter()
		};
	}

	Point Bounds::getTopLeft() const {
		return {
			getX(),
			getY()
		};
	}

	Point Bounds::getTopRight() const {
		return {
			getX2(),
			getY()
		};
	}

	Point Bounds::getBottomRight() const {
		return {
			getX2(),
			getY2()
		};
	}

	Point Bounds::getBottomLeft() const {
		return {
			getX(),
			getY2()
		};
	}

	bool Bounds::intersects(const Point& point) const {
		return
			point.getX() >= getX()
			&& point.getY() >= getY()
			&& point.getX() <= getX2()
			&& point.getY() <= getY2();
	}

	bool Bounds::intersects(const Bounds& bounds) const {
		return !(
			getX() > bounds.getX2()
			|| getX2() < bounds.getX()
			|| getY() > bounds.getY2()
			|| getY2() < bounds.getY()
		);
	}

	bool Bounds::isNonZero() const {
		return getWidth() > 0 && getHeight() > 0;
	}

	Bounds Bounds::getIntersection(const Bounds& bounds) const {
		Bounds result = Bounds();

		result.setX(max(getX(), bounds.getX()));
		result.setY(max(getY(), bounds.getY()));
		result.setWidth(min(getX2(), bounds.getX2()) - result.getX() + 1);
		result.setHeight(min(getY2(), bounds.getY2()) - result.getY() + 1);

		return result;
	}
}