#include <YOBA/UI/Decorator.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>

namespace YOBA {
	void Decorator::handleEvent(Event* event, const Rectangle& parentBounds, bool callHandlers) {
		const auto isPointer = PointerEvent::isPointer(event);
		const auto isPinch = PinchEvent::isPinch(event);
		auto currentBounds = getRenderBounds();

		if (isPointer || isPinch) {
			bool callHandlersOnThis;

			if (isVisible() && isVisibleForPointerEvents() && isEnabled()) {
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
					currentBounds = Rectangle::invalidValue;
				}

				const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;

				setPointerOver(
					event->getTypeID() != PointerUpEvent::typeID
					&& event->getTypeID() != PinchUpEvent::typeID
					&& contains
					&& (
						!capturedElement
						|| capturedElement == this
					)
				);

				if (isEnabled()) {
					callHandlersOnThis =
						callHandlers
						&& (
							(
								contains
								&& !capturedElement
							)
							|| capturedElement == this
						);
				}
				else {
					callHandlers = false;
					callHandlersOnThis = false;
				}
			}
			else {
				setPointerOver(false);

				callHandlers = false;
				callHandlersOnThis = false;
			}

			// Before children
			if (callHandlersOnThis) {
				onEventBeforeChild(event);

				if (event->isHandled()) {
					callHandlers = false;
					callHandlersOnThis = false;
				}
			}

			// Children
			if (_child) {
				_child->handleEvent(event, currentBounds, callHandlers);

				if (event->isHandled()) {
					callHandlers = false;
					callHandlersOnThis = false;
				}
			}

			// After children
			if (callHandlersOnThis)
				onEventAfterChild(event);
		}
		else {
			if (!isVisible() || !isEnabled())
				return;

			const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;
			const auto callHandlersOnThis = callHandlers && (!capturedElement || capturedElement == this);

			// ESP_LOGI("layout.handleEvent()", "callHandlersOnThis 1: %d", callHandlersOnThis);

			// Before
			if (callHandlersOnThis)
				onEventBeforeChild(event);

			if (event->isHandled())
				return;

			// Children
			if (_child) {
				_child->handleEvent(event, currentBounds, callHandlers);

				if (event->isHandled())
					return;
			}

			// ESP_LOGI("layout.handleEvent()", "callHandlersOnThis 2: %d", callHandlersOnThis);

			// After
			if (callHandlersOnThis)
				onEventAfterChild(event);
		}
	}
}
