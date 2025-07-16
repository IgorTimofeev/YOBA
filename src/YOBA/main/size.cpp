#include "size.h"

namespace YOBA {
	Size::Size(const uint16_t width, const uint16_t height) :
		width(width),
		height(height)
	{

	}

	Size::Size(const uint16_t uniformValue) : Size(uniformValue, uniformValue) {

	}

	Size::Size() : Size(0, 0) {

	}

	uint16_t Size::getWidth() const {
		return width;
	}

	void Size::setWidth(const uint16_t value) {
		width = value;
	}

	uint16_t Size::getHeight() const {
		return height;
	}

	void Size::setHeight(const uint16_t value) {
		height = value;
	}

	uint16_t Size::getXCenter() const {
		return width / 2;
	}

	uint16_t Size::getYCenter() const {
		return height / 2;
	}

	Point Size::getCenter() const {
		return {
			getXCenter(),
			getYCenter()
		};
	}

	uint32_t Size::getSquare() const {
		return width * height;
	}

	Size Size::interpolate(const Size& to, const double& position) const {
		return {
			static_cast<uint16_t>(getWidth() + position * (to.getWidth() - getWidth())),
			static_cast<uint16_t>(getHeight() + position * (to.getHeight() - getHeight()))
		};
	}

	bool Size::operator==(const Size& rhs) const {
		return width == rhs.width && height == rhs.height;
	}

	bool Size::operator!=(const Size& rhs) const {
		return !(rhs == *this);
	}
}