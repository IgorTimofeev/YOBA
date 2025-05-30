#include <esp_log.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/application.h>
#include <YOBA/main/events/pointerEvent.h>

namespace YOBA {
	void Control::handleEvent(Event* event, bool callHandlers) {
		if (PointerEvent::isPointer(event)) {
			if (isVisible() && isVisibleForPointerEvents()) {
				const auto intersects = getBounds().intersects(reinterpret_cast<PointerEvent*>(event)->getPosition());
				const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;

				setPointerOver(
					event->getTypeID() != PointerUpEvent::typeID
					&& intersects
					&& (
						!capturedElement
						|| capturedElement == this
					)
				);

				if (
					callHandlers
					&& isEnabled()
					&& (
						(
							intersects
							&& !capturedElement
						)
						|| capturedElement == this
					)
				) {
					onEvent(event);
				}
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
