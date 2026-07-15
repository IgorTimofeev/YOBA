#include <YOBA/Core/Events/ScreenEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/MouseWheelEvent.hpp>

namespace YOBA {
	ScreenEvent::ScreenEvent(const uint16_t typeID) : Event(typeID) {

	}

	bool ScreenEvent::isScreen(const Event* event) {
		return
			event->is<PointerDownEvent>()
			|| event->is<PointerDragEvent>()
			|| event->is<PointerUpEvent>()

			|| event->is<PinchDownEvent>()
			|| event->is<PinchDragEvent>()
			|| event->is<PinchUpEvent>()

			|| event->is<MouseWheelEvent>();
	}
}
