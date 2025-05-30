#include "pointerEvent.h"

#include <cstdint>

namespace YOBA {
	PointerEvent::PointerEvent(uint16_t& staticTypeID, const Point& position) :
		ScreenEvent(staticTypeID),
		_position(position)
	{

	}

	const Point& PointerEvent::getPosition() const {
		return _position;
	}

	void PointerEvent::setPosition(const Point& position) {
		_position = position;
	}

	bool PointerEvent::isPointer(const Event* event) {
		return
			event->getTypeID() == PointerDownEvent::typeID
			|| event->getTypeID() == PointerDragEvent::typeID
			|| event->getTypeID() == PointerUpEvent::typeID;
	}

	PointerDownEvent::PointerDownEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerDownEvent::typeID = 0;

	PointerDragEvent::PointerDragEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerDragEvent::typeID = 0;

	PointerUpEvent::PointerUpEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerUpEvent::typeID = 0;
}
