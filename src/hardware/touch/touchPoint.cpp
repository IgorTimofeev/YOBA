#include "touchPoint.h"

namespace yoba {
	const Point& TouchPoint::getPosition() const {
		return _position;
	}

	void TouchPoint::setPosition(const Point& position) {
		_position = position;
	}

	bool TouchPoint::isDown() const {
		return _isDown;
	}

	void TouchPoint::setDown(bool isDown) {
		_isDown = isDown;
	}
}
