#include "application.h"
#include "element.h"
#include "animation.h"

namespace yoba::ui {
	Application::Application(){
		Layout::setApplication(this);
	}

	void Application::setRenderer(Renderer* value) {
		_renderer = value;

		 invalidate();
	}

	void Application::setup(RenderTarget* renderTarget, Renderer* renderer) {
		renderTarget->setup();
		renderer->setTarget(renderTarget);
		setRenderer(renderer);
	}

	void Application::setup(RenderTarget* renderTarget, Renderer* renderer, hardware::InputDevice* inputDevice) {
		setup(renderTarget, renderer);

		inputDevice->setup();
		addInputDevice(inputDevice);
	}

	void Application::invalidateLayout() {
		_layoutInvalidated = false;
	}

	void Application::invalidateRender() {
		_renderInvalidated = false;
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
		// Root element size should match render target size
		setSize(_renderer->getTarget()->getSize());

		// Resetting viewport just in case if some UI element broke it
		_renderer->resetViewport();

		auto time = system::getTime();

		// Handling input from devices like touchscreens, rotary encoders, etc.
		inputDevicesTick();

		_peripheralsDeltaTime = system::getTime() - time;

		time = system::getTime();

		// Handling child elements onTick()
		onTick();

		// Playing animations
		animationsTick();

		_tickDeltaTime = system::getTime() - time;

		// Measuring size of children
		if (_layoutInvalidated) {
			time = system::getTime();
			measure(getSize());
			_layoutDeltaTime = system::getTime() - time;

			_layoutInvalidated = false;
		}

		if (_renderInvalidated) {
			// Rendering children
			time = system::getTime();
			render(_renderer, Bounds(getSize()));
			_renderDeltaTime = system::getTime() - time;

			// Flushing screen buffer
			time = system::getTime();
			_renderer->flushBuffer();
			_flushDeltaTime = system::getTime() - time;

			_renderInvalidated = false;
		}

		// Running enqueued tasks if any
		if (!_enqueuedTasksOnTick.empty()) {
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

	uint32_t Application::getTickDeltaTime() const {
		return _tickDeltaTime;
	}

	uint32_t Application::getLayoutDeltaTime() const {
		return _layoutDeltaTime;
	}

	uint32_t Application::getRenderDeltaTime() const {
		return _renderDeltaTime;
	}

	uint32_t Application::getFlushDeltaTime() const {
		return _flushDeltaTime;
	}

	void Application::onRender(Renderer* renderer, const Bounds& bounds) {
		if (getBackgroundColor())
			renderer->clear(getBackgroundColor());

		Layout::onRender(renderer, bounds);
	}

	uint32_t Application::getPeripheralsDeltaTime() const {
		return _peripheralsDeltaTime;
	}
}