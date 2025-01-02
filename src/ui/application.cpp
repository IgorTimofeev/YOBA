#include "application.h"
#include "element.h"
#include "animation.h"

namespace yoba {
	const unscii16Font Application::defaultFont = unscii16Font();

	Application::Application(
		ScreenBuffer* screenBuffer
	) :
		_screenBuffer(screenBuffer)
	{
		Container::setApplication(this);
	}

	void Application::setup() {
		_screenBuffer->setup();
	}

	void Application::invalidateLayout() {
		_isMeasuredAndArranged = false;
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
		setSize(_screenBuffer->getSize());

		Container::tick();

		// Handling tick handlers first
		_onTick.call();

		// Playing animations
		animationsTick();

		// Computing sizes & positions
		if (!_isMeasuredAndArranged) {
			measure(_screenBuffer, getSize());
			arrange(Bounds(getSize()));

			_isMeasuredAndArranged = true;
		}

		// Rendering
		if (!_isRendered) {
			render(_screenBuffer);

			_screenBuffer->flush();

			_isRendered = true;
		}
	}

	Element* Application::getCapturedElement() const {
		return _capturedElement;
	}

	void Application::setCapturedElement(Element* capturedElement) {
		_capturedElement = capturedElement;
	}

	Element* Application::getFocusedElement() const {
		return _focusedElement;
	}

	void Application::setFocusedElement(Element* focusedElement) {
		_focusedElement = focusedElement;
	}

	void Application::handleEvent(InputEvent &event) {
		if (getCapturedElement()) {
			getCapturedElement()->handleEvent(event);
		}
		else {
			Element::handleEvent(event);
		}
	}

	const Font* Application::getFont() const {
		return _defaultFont;
	}

	void Application::setFont(const Font* font) {
		_defaultFont = font;
	}

	ScreenOrientation Application::getOrientation() const {
		return _screenBuffer->getDriver()->getOrientation();
	}

	void Application::setOrientation(ScreenOrientation value) const {
		return _screenBuffer->getDriver()->setOrientation(value);
	}

	Callback<>& Application::getOnTick() {
		return _onTick;
	}

	ScreenBuffer* Application::getScreenBuffer() const {
		return _screenBuffer;
	}

	void Application::addInputDevice(InputDevice* inputDevice) {
		_onTick += [this, inputDevice]() {
			inputDevice->tick(this);
		};
	}
}