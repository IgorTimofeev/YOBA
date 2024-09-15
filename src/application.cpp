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
	}

	void Application::begin() {
		_screenBuffer->begin();
		_touchPanel.begin();
	}

	void Application::tick() {
		if (millis() <= _tickDeadline)
			return;

		_touchPanel.tick([&](Event& event) {
			_workspace.handleEvent(event);
		});

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