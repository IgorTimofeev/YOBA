#include <YOBA/main/events/pointerEvent.h>

namespace YOBA {
	PointerEvent::PointerEvent(const uint16_t typeID, const Point& position) :
		ScreenEvent(typeID),
		_position(position)
	{

	}

	const Point& PointerEvent::getPosition() const {
		return _position;
	}

	void PointerEvent::setPosition(const Point& position) {
		_position = position;
	}

	bool PointerEvent::isPointer(const Event& event) {
		return
			event.getTypeID() == PointerDownEvent::typeID
			|| event.getTypeID() == PointerDragEvent::typeID
			|| event.getTypeID() == PointerUpEvent::typeID;
	}

	PointerDownEvent::PointerDownEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerDownEvent::typeID = registerTypeID();

	PointerDragEvent::PointerDragEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerDragEvent::typeID = registerTypeID();

	PointerUpEvent::PointerUpEvent(const Point& position) : PointerEvent(
		typeID,
		position
	) {

	}

	uint16_t PointerUpEvent::typeID = registerTypeID();
}
