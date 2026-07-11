#include <YOBA/Core/Events/MouseWheelEvent.hpp>

namespace YOBA {
	MouseWheelEvent::MouseWheelEvent(const Point& position, const int32_t delta) : PointerEvent(typeID, position), _delta(delta) {

	}

	int32_t MouseWheelEvent::getDelta() const {
		return _delta;
	}

	void MouseWheelEvent::setDelta(const int32_t delta) {
		_delta = delta;
	}

	uint16_t MouseWheelEvent::typeID = registerTypeID();
}