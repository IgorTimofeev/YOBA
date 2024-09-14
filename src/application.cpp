#include "application.h"
#include "point.h"
#include "event.h"

namespace yoba {
	Application::Application(
		ScreenBuffer* screenBuffer,
		TouchDriver* touchDriver
	) :
		_screenBuffer(screenBuffer),
		_touchPanel(TouchPanel(touchDriver))
	{
		_workspace.setSize(screenBuffer->getSize());

		// Touch
		_touchPanel.addOnTouchDown([this](const Point& point) {
			auto event = TouchDownEvent(point);
			_workspace.handleEvent(event);
		});

		_touchPanel.addOnTouchDrag([this](const Point& point) {
			auto event = TouchDragEvent(point);
			_workspace.handleEvent(event);
		});

		_touchPanel.addOnTouchUp([this](const Point& point) {
			auto event = TouchUpEvent(point);
			_workspace.handleEvent(event);
		});

		// Pinch
		_touchPanel.addOnPinchDown([this](const Point& point1, const Point& point2) {
			auto event = PinchDownEvent(point1, point2);
			_workspace.handleEvent(event);
		});

		_touchPanel.addOnPinchDrag([this](const Point& point1, const Point& point2) {
			auto event = PinchDragEvent(point1, point2);
			_workspace.handleEvent(event);
		});

		_touchPanel.addOnPinchUp([this](const Point& point1, const Point& point2) {
			auto event = PinchUpEvent(point1, point2);
			_workspace.handleEvent(event);
		});
	}

	void Application::begin() {
		_screenBuffer->begin();
		_touchPanel.begin();
	}

	void Application::tick() {
		if (millis() <= _tickDeadline)
			return;

		_touchPanel.readTouch();
		_workspace.tick();
		_workspace.measure(_screenBuffer);
		_workspace.arrange();
		_workspace.render(_screenBuffer);

		_tickDeadline = millis() + _tickInterval;
	}

	uint32_t Application::getTickInterval() const {
		return _tickInterval;
	}

	void Application::setTickInterval(uint32_t tickInterval) {
		_tickInterval = tickInterval;
	}
}