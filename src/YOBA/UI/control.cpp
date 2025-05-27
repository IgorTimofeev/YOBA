#include <YOBA/UI/control.h>

namespace YOBA {
	void Control::pushEvent(Event* event) {
		if (!isVisible() || !updateIsTouchOverAndCheckIfShouldHandleEvent(event))
			return;

		onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			onTouchDown(reinterpret_cast<TouchDownEvent*>(event));
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			onTouchDrag(reinterpret_cast<TouchDragEvent*>(event));
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			onTouchUp(reinterpret_cast<TouchUpEvent*>(event));
		}
		else if (event->getTypeID() == PinchDownEvent::typeID) {
			onPinchDown(reinterpret_cast<PinchDownEvent*>(event));
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			onPinchDrag(reinterpret_cast<PinchDragEvent*>(event));
		}
		else if (event->getTypeID() == PinchUpEvent::typeID) {
			onPinchUp(reinterpret_cast<PinchUpEvent*>(event));
		}
	}

	void Control::onEvent(Event* event) {

	}

	void Control::onTouchDown(TouchDownEvent* event) {
		event->setHandled(true);
	}

	void Control::onTouchDrag(TouchDragEvent* event) {
		event->setHandled(true);
	}

	void Control::onTouchUp(TouchUpEvent* event) {
		event->setHandled(true);
	}

	void Control::onPinchDown(PinchDownEvent* event) {
		event->setHandled(true);
	}

	void Control::onPinchDrag(PinchDragEvent* event) {
		event->setHandled(true);
	}

	void Control::onPinchUp(PinchUpEvent* event) {
		event->setHandled(true);
	}
}
