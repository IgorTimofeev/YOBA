#include "size.h"

namespace yoba {
	Size::Size(uint16_t width, uint16_t height) :
		_width(width),
		_height(height)
	{

	}

	Size::Size(uint16_t uniformValue) : Size(uniformValue, uniformValue) {

	}

	Size::Size() : Size(0, 0) {

	}

	uint16_t Size::getWidth() const {
		return _width;
	}

	void Size::setWidth(uint16_t value) {
		_width = value;
	}

	uint16_t Size::getHeight() const {
		return _height;
	}

	void Size::setHeight(uint16_t value) {
		_height = value;
	}

	uint16_t Size::getXCenter() const {
		return _width / 2;
	}

	uint16_t Size::getYCenter() const {
		return _height / 2;
	}

	Point Size::getCenter() const {
		return {
			getXCenter(),
			getYCenter()
		};
	}

	uint32_t Size::getSquare() const {
		return _width * _height;
	}

	Size Size::interpolate(const Size& to, const double& position) const {
		return {
			(uint16_t) (getWidth() + position * (to.getWidth() - getWidth())),
			(uint16_t) (getHeight() + position * (to.getHeight() - getHeight()))
		};
	}

	bool Size::operator==(const Size& rhs) const {
		return _width == rhs._width && _height == rhs._height;
	}

	bool Size::operator!=(const Size& rhs) const {
		return !(rhs == *this);
	}
}