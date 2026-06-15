#include <YOBA/core/events/screenEvent.h>

#include <YOBA/core/events/pinchEvent.h>
#include <YOBA/core/events/pointerEvent.h>

namespace YOBA {
	ScreenEvent::ScreenEvent(const uint16_t typeID) : Event(typeID) {

	}

	bool ScreenEvent::isScreen(const Event* event) {
		return
			event->getTypeID() == PointerDownEvent::typeID
			|| event->getTypeID() == PointerDragEvent::typeID
			|| event->getTypeID() == PointerUpEvent::typeID

			|| event->getTypeID() == PinchDownEvent::typeID
			|| event->getTypeID() == PinchDragEvent::typeID
			|| event->getTypeID() == PinchUpEvent::typeID;
	}
}
