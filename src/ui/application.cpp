#include "application.h"
#include "element.h"
#include "animation.h"

namespace yoba::ui {
	Application::Application(Renderer* renderer) : _renderer(renderer) {
		Layout::setApplication(this);
	}

	void Application::setup() {
		_renderer->setup();
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
		setSize(_renderer->getRenderTarget()->getResolution());

		// Resetting viewport just in case if some UI element broke it
		_renderer->resetViewport();

		// Handling input from devices like touchscreens, rotary encoders, etc.
		inputDevicesTick();

		// Handling child elements onTick()
		onTick();

		// Playing animations
		animationsTick();

		// Measuring & arranging child elements
		if (!_isMeasuredAndArranged) {
			measure(getSize());
			arrange(Bounds(getSize()));

			_isMeasuredAndArranged = true;
		}

		// Rendering
		if (!_isRendered) {
			render(_renderer);
			_renderer->flush();

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

	void Application::handleEvent(Event& event) {
		if (getCapturedElement()) {
			getCapturedElement()->handleEvent(event);
		}
		else {
			Element::handleEvent(event);
		}
	}

	Renderer* Application::getRenderer() const {
		return _renderer;
	}

	void Application::addInputDevice(hardware::InputDevice* inputDevice) {
		_inputDevices.push_back(inputDevice);
	}

	void Application::enqueueOnTick(const std::function<void()>& task) {
		_enqueuedTasksOnTick.push_back(task);
	}

	void Application::inputDevicesTick() {
		for (auto inputDevice : _inputDevices)
			inputDevice->tick(this);
	}
}