#pragma once

#include <YOBA/Core/Events/ScreenEvent.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/UI/Dialogs/Dialog.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	const std::function<void()>& DialogOverlayShape::getOnPointerEvent() const {
		return _onPointerEvent;
	}

	void DialogOverlayShape::setOnPointerEvent(const std::function<void()>& onPointerDown) {
		_onPointerEvent = onPointerDown;
	}

	void DialogOverlayShape::onEvent(Event* event) {
		if (!ScreenEvent::isScreen(event))
			return;

		if (event->is<PointerUpEvent>()) {
			if (_onPointerEvent)
				_onPointerEvent();
		}

		event->setHandled(true);
	}
}
