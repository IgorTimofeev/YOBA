#include <esp_log.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/application.h>

#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/main/events/pinchEvent.h>

namespace YOBA {
	void Control::handleEvent(Event* event, const Bounds& parentBounds, const bool callHandlers) {
		const auto isPointer = PointerEvent::isPointer(event);
		const auto isPinch = PinchEvent::isPinch(event);

		if (isPointer || isPinch) {
			if (isVisible() && isVisibleForPointerEvents() && isEnabled()) {
				auto currentBounds = getBounds();
				bool contains;

				if (parentBounds.intersects(currentBounds)) {
					currentBounds = parentBounds.getIntersection(currentBounds);

					if (isPointer) {
						contains = currentBounds.contains(reinterpret_cast<PointerEvent*>(event)->getPosition());
					}
					else {
						const auto pinchEvent = reinterpret_cast<PinchEvent*>(event);
						contains = currentBounds.contains(pinchEvent->getPosition1()) && currentBounds.contains(pinchEvent->getPosition2());
					}
				}
				else {
					contains = false;
				}

				const auto capturedElement =
					Application::hasCurrent() && Application::getCurrent().hasCapturedElement()
					? &Application::getCurrent().getCapturedElement()
					: nullptr;

				setPointerOver(
					event->getTypeID() != PointerUpEvent::typeID
					&& event->getTypeID() != PinchUpEvent::typeID
					&& contains
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
							contains
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

			const auto capturedElement =
				Application::hasCurrent() && Application::getCurrent().hasCapturedElement()
				? &Application::getCurrent().getCapturedElement()
				: nullptr;

			if (!capturedElement || capturedElement == this)
				onEvent(event);
		}
	}

	void Control::onEvent(Event* event) {
		if (ScreenEvent::isScreen(event))
			event->setHandled(true);
	}
}
