#include "bounds.h"

namespace YOBA {
	Bounds::Bounds(int32_t x, int32_t y, int32_t width, int32_t height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{

	}

	Bounds::Bounds() : Bounds(0, 0, 0, 0) {

	}

	Bounds::Bounds(const Point& position, const Size& size) : Bounds(
		position.getX(),
		position.getY(),
		size.getWidth(),
		size.getHeight()
	) {

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

	Point Bounds::getPosition() const {
		return {
			_x,
			_y
		};
	}

	void Bounds::setPosition(const Point& value) {
		_x = value.getX();
		_y = value.getY();
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

	Size Bounds::getSize() const {
		return {
			_width,
			_height
		};
	}

	void Bounds::setSize(const Size& value) {
		_width = value.getWidth();
		_height = value.getHeight();
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

	uint32_t Bounds::getSquare() const {
		return _width * _height;
	}

	Point Bounds::getTopLeft() const {
		return getPosition();
	}

	Point Bounds::getTopRight() const {
		return {
			getX2(),
			_y
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
			_x,
			getY2()
		};
	}

	bool Bounds::intersects(const Point& point) const {
		return
			point.getX() >= _x
			&& point.getY() >= _y
			&& point.getX() <= getX2()
			&& point.getY() <= getY2();
	}

	bool Bounds::intersects(const Bounds& bounds) const {
		return !(
			_x > bounds.getX2()
			|| getX2() < bounds._x
			|| _y > bounds.getY2()
			|| getY2() < bounds._y
		);
	}

	bool Bounds::haveZeroSize() const {
		return getWidth() == 0 || getHeight() == 0;
	}

	bool Bounds::isLandscape() const {
		return getWidth() > getHeight();
	}

	Bounds Bounds::getIntersection(const Bounds& bounds) const {
		auto result = Bounds();

		result._x = std::max(_x, bounds._x);
		result._y = std::max(_y, bounds._y);
		result._width = std::min(_x + _width, bounds._x + bounds._width) - result._x;
		result._height = std::min(_y + _height, bounds._y + bounds._height) - result._y;

		return result;
	}

	Bounds Bounds::getExpansion(const Bounds& bounds) const {
		auto result = Bounds();

		result._x = std::min(_x, bounds._x);
		result._x = std::min(_y, bounds._y);
		result._width = std::max(_x + _width, bounds._x + bounds._width) - result._x;
		result._height = std::max(_y + _height, bounds._y + bounds._height) - result._y;

		return result;
	}
}