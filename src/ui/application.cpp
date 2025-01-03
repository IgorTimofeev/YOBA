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
		// Root element size should match screen size
		setSize(_screenBuffer->getSize());

		Container::tick();

		// Handling input from devices like touchscreens, rotary encoders, etc
		for (auto inputDevice : _inputDevices)
			inputDevice->tick(this);

		// Playing animations
		animationsTick();

		// Measuring & arranging child elements
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

		// Running enqueued tasks if any
		if (_enqueuedTasksOnTick.size() > 0) {
			for (const auto& task : _enqueuedTasksOnTick)
				task();

			_enqueuedTasksOnTick.clear();
		}
	}

	Element* Application::getCapturedElement() const {
		return _capturedElement;
	}

	void Application::setCapturedElement(Element* element) {
		const auto oldCapturedElement = _capturedElement;

		_capturedElement = element;

		if (oldCapturedElement && oldCapturedElement != _capturedElement)
			oldCapturedElement->onCaptureChanged();

		if (_capturedElement && _capturedElement != oldCapturedElement)
			_capturedElement->onCaptureChanged();

		invalidate();
	}

	Element* Application::getFocusedElement() const {
		return _focusedElement;
	}

	void Application::setFocusedElement(Element* element) {
		const auto oldFocusedElement = _focusedElement;

		_focusedElement = element;

		if (oldFocusedElement && oldFocusedElement != _focusedElement)
			oldFocusedElement->onFocusChanged();

		if (_focusedElement && _focusedElement != oldFocusedElement)
			_focusedElement->onFocusChanged();

		invalidate();
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

	ScreenBuffer* Application::getScreenBuffer() const {
		return _screenBuffer;
	}

	void Application::addInputDevice(InputDevice* inputDevice) {
		_inputDevices.push_back(inputDevice);
	}

	void Application::enqueueOnTick(const std::function<void()>& task) {
		_enqueuedTasksOnTick.push_back(task);
	}
}