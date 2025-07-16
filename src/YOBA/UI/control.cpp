#include <esp_log.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/application.h>

#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/main/events/pinchEvent.h>

namespace YOBA {
	void Control::handleEvent(Event* event, const bool callHandlers) {
		const auto isPointer = PointerEvent::isPointer(event);
		const auto isPinch = PinchEvent::isPinch(event);

		if (isPointer || isPinch) {
			if (isVisible() && isVisibleForPointerEvents()) {
				bool intersects;

				if (isPointer) {
					intersects = getBounds().contains(reinterpret_cast<PointerEvent*>(event)->getPosition());
				}
				else {
					const auto& bounds = getBounds();
					const auto pinchEvent = reinterpret_cast<PinchEvent*>(event);
					intersects = bounds.contains(pinchEvent->getPosition1()) && bounds.contains(pinchEvent->getPosition2());
				}

				const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;

				setPointerOver(
					event->getTypeID() != PointerUpEvent::typeID
					&& event->getTypeID() != PinchUpEvent::typeID
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
			if (!isVisible() || !isEnabled())
				return;

			const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;

			if (!capturedElement || capturedElement == this)
				onEvent(event);
		}
	}

	void Control::onEvent(Event* event) {
		if (ScreenEvent::isScreen(event))
			event->setHandled(true);
	}
}
