#include <YOBA/UI/control.h>
#include <YOBA/main/event.h>

namespace YOBA {
	void Control::handleEvent(Event* event, bool callHandlers) {
		if (TouchEvent::isTouch(event)) {
			if (isVisible() && isVisibleForPointerEvents()) {
				setPointerOver(getBounds().intersects(reinterpret_cast<TouchEvent*>(event)->getPosition()));

				if (callHandlers && isEnabled() && (isPointerOver() || isCaptured()))
					onEvent(event);
			}
			else {
				setPointerOver(false);
			}
		}
		else {
			if (isVisible() && isEnabled())
				onEvent(event);
		}
	}

	void Control::onEvent(Event* event) {
		if (ScreenEvent::isScreen(event))
			event->setHandled(true);
	}
}
