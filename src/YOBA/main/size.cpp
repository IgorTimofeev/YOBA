#include "size.h"

namespace YOBA {
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

	Size Size::interpolate(const Size& to, const float& position) const {
		return {
			static_cast<uint16_t>(static_cast<int32_t>(getWidth()) + static_cast<int32_t>(position * static_cast<float>(static_cast<int32_t>(to.getWidth()) - static_cast<int32_t>(getWidth())))),
			static_cast<uint16_t>(static_cast<int32_t>(getHeight()) + static_cast<int32_t>(position * static_cast<float>(static_cast<int32_t>(to.getHeight()) - static_cast<int32_t>(getHeight()))))
		};
	}

	bool Size::operator==(const Size& rhs) const {
		return width == rhs.width && height == rhs.height;
	}

	bool Size::operator!=(const Size& rhs) const {
		return !(rhs == *this);
	}
}