#include <YOBA/UI/control.h>

namespace YOBA {
	void Control::pushEvent(Event* event) {
		if (!isVisible())
			return;

		callEventSpecificFunctions(
			this,

			&Control::onTouchDown,
			&Control::onTouchDrag,
			&Control::onTouchUp,

			&Control::onPinchDown,
			&Control::onPinchDrag,
			&Control::onPinchUp,

			event
		);

		onEvent(event);
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
