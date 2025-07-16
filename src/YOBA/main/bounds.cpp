#include "bounds.h"

namespace YOBA {
	Bounds::Bounds(const int32_t x, const int32_t y, const int32_t width, const int32_t height) :
		x(x),
		y(y),
		width(width),
		height(height)
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
		return x;
	}

	void Bounds::setX(const int32_t value) {
		x = value;
	}

	int32_t Bounds::getY() const {
		return y;
	}

	void Bounds::setY(const int32_t value) {
		y = value;
	}

	Point Bounds::getPosition() const {
		return {
			x,
			y
		};
	}

	void Bounds::setPosition(const Point& value) {
		x = value.getX();
		y = value.getY();
	}

	uint16_t Bounds::getWidth() const {
		return width;
	}

	void Bounds::setWidth(const uint16_t value) {
		width = value;
	}

	uint16_t Bounds::getHeight() const {
		return height;
	}

	void Bounds::setHeight(const uint16_t value) {
		height = value;
	}

	Size Bounds::getSize() const {
		return {
			width,
			height
		};
	}

	void Bounds::setSize(const Size& value) {
		width = value.getWidth();
		height = value.getHeight();
	}

	int32_t Bounds::getX2() const {
		return x + width - 1;
	}

	int32_t Bounds::getY2() const {
		return y + height - 1;
	}

	int32_t Bounds::getXCenter() const {
		return x + width / 2;
	}

	int32_t Bounds::getYCenter() const {
		return y + height / 2;
	}

	Point Bounds::getCenter() const {
		return {
			getXCenter(),
			getYCenter()
		};
	}

	uint32_t Bounds::getSquare() const {
		return width * height;
	}

	Point Bounds::getTopLeft() const {
		return getPosition();
	}

	Point Bounds::getTopRight() const {
		return {
			getX2(),
			y
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
			x,
			getY2()
		};
	}

	bool Bounds::intersects(const Bounds& bounds) const {
		return !(
			x > bounds.getX2()
			|| getX2() < bounds.x
			|| y > bounds.getY2()
			|| getY2() < bounds.y
		);
	}

	bool Bounds::contains(const Point& point) const {
		return
			point.getX() >= x
			&& point.getY() >= y
			&& point.getX() <= getX2()
			&& point.getY() <= getY2();
	}

	bool Bounds::contains(const Bounds& bounds) const {
		return
			bounds.getX() >= x
			&& bounds.getY() >= y
			&& bounds.getX2() <= getX2()
			&& bounds.getY2() <= getY2();
	}

	bool Bounds::haveZeroSize() const {
		return getWidth() == 0 || getHeight() == 0;
	}

	bool Bounds::isLandscape() const {
		return getWidth() > getHeight();
	}

	Bounds Bounds::getIntersection(const Bounds& bounds) const {
		auto result = Bounds();

		result.x = std::max(x, bounds.x);
		result.y = std::max(y, bounds.y);
		result.width = std::min(x + width, bounds.x + bounds.width) - result.x;
		result.height = std::min(y + height, bounds.y + bounds.height) - result.y;

		return result;
	}

	Bounds Bounds::getExpansion(const Bounds& bounds) const {
		auto result = Bounds();

		result.x = std::min(x, bounds.x);
		result.x = std::min(y, bounds.y);
		result.width = std::max(x + width, bounds.x + bounds.width) - result.x;
		result.height = std::max(y + height, bounds.y + bounds.height) - result.y;

		return result;
	}
}