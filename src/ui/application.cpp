#include "application.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "element.h"
#include "animation.h"

namespace yoba {
	Application::Application(
		ScreenBuffer* screenBuffer,
		TouchDriver* touchDriver
	) :
		_screenBuffer(screenBuffer),
		_touchDriver(touchDriver)
	{

	}

	void Application::setup() {
		Layout::setRoot(this);

		_screenBuffer->setup();
		_touchDriver->setup();
	}

	void Application::invalidateLayout() {
		_isMeasured = false;
		_isArranged = false;
	}

	void Application::invalidateRender() {
		_isRendered = false;
	}

	void Application::invalidate() {
		invalidateLayout();
		invalidateRender();
	}

	void Application::startAnimation(Animation* animation) {
		_animations.push_back(animation);

		animation->start();
	}

	void Application::animationsTick() {
		if (_animations.empty())
			return;

		for (int i = 0; i < _animations.size(); i++) {
			if (_animations[i]->tick())
				continue;

			_animations[i]->stop();
			delete _animations[i];
			_animations.erase(_animations.begin() + i);

			i--;
		}
	}

	void Application::tick() {
		setSize(_screenBuffer->getDriver()->getResolution());

		// Handling input events first
		_touchDriver->tick(_screenBuffer, [&](InputEvent& event) {
			handleEvent(event);
		});

		// Playing animations
		animationsTick();

		// Computing sizes
		if (!_isMeasured) {
			measure(_screenBuffer, getSize());

			_isMeasured = true;
		}

		// Computing positions
		if (!_isArranged) {
			arrange(Bounds(getSize()));

			_isArranged = true;
		}

		// Rendering
		if (!_isRendered) {
			render(_screenBuffer);

			_screenBuffer->flush();

			_isRendered = true;
		}
	}

	Element *Application::getCapturedElement() const {
		return _capturedElement;
	}

	void Application::setCapturedElement(Element *capturedElement) {
		_capturedElement = capturedElement;
	}

	void Application::handleEvent(InputEvent &event) {
		if (getCapturedElement()) {
			getCapturedElement()->handleEvent(event);
		}
		else {
			Element::handleEvent(event);
		}
	}

	const Font *Application::getDefaultFont() const {
		return _defaultFont;
	}

	void Application::setDefaultFont(const Font *defaultFont) {
		_defaultFont = defaultFont;
	}

	ScreenOrientation Application::getOrientation() const {
		return _screenBuffer->getDriver()->getOrientation();
	}

	void Application::setOrientation(ScreenOrientation value) const {
		return _screenBuffer->getDriver()->setOrientation(value);
	}
}