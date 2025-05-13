#include "application.h"

#include "YOBA/UI/element.h"
#include "animation.h"

namespace YOBA {
	Application* Application::_current = nullptr;

	Application* Application::getCurrent() {
		return _current;
	}

	Application::Application() {

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

	void Application::setup(RenderTarget* renderTarget, Renderer* renderer, InputDevice* inputDevice) {
		setup(renderTarget, renderer);

		inputDevice->setup();
		addInputDevice(inputDevice);
	}

	void Application::invalidateLayout() {
		_layoutInvalidated = true;
	}

	void Application::invalidateRender() {
		_renderInvalidated = true;

//		esp_backtrace_print(10);
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
		_current = this;

		// Root element size should match render target size
		setSize(_renderer->getTarget()->getSize());

		// Resetting viewport just in case if some UI element broke it
		_renderer->resetViewport();

		auto time = system::getTime();

		// Handling input from devices like touchscreens, rotary encoders, etc.
		inputDevicesTick();

		_peripheralsDeltaTime = system::getTime() - time;

		time = system::getTime();

		// Handling tick for children
		onTick();

		// Running enqueued tasks
		if (!_scheduledTasks.empty()) {
			for (const auto& callback : _scheduledTasks)
				callback();

			_scheduledTasks.clear();
		}

		// Playing animations
		animationsTick();

		_tickDeltaTime = system::getTime() - time;

		// Measuring children size
		if (_layoutInvalidated) {
			time = system::getTime();
			measure(getSize());
			_layoutDeltaTime = system::getTime() - time;

			_layoutInvalidated = false;
		}

		// Render pass
		if (_renderInvalidated) {
			// Rendering
			time = system::getTime();
			render(_renderer, getBounds());
			_renderDeltaTime = system::getTime() - time;

			// Flushing screen buffer
			time = system::getTime();
			_renderer->flush();
			_flushDeltaTime = system::getTime() - time;

			_renderInvalidated = false;
		}
	}

	Element* Application::getCapturedElement() const {
		return _capturedElement;
	}

	void Application::setCapturedElement(Element* element) {
		_capturedElement = element;
	}

	Element* Application::getFocusedElement() const {
		return _focusedElement;
	}

	void Application::setFocusedElement(Element* element) {
		_focusedElement = element;
	}

	void Application::handleEvent(Event* event) {
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

	void Application::addInputDevice(InputDevice* inputDevice) {
		_inputDevices.push_back(inputDevice);
	}

	void Application::scheduleTask(const std::function<void()>& task) {
		_scheduledTasks.push_back(task);
	}

	void Application::inputDevicesTick() {
		for (const auto inputDevice : _inputDevices)
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

	void Application::onRender(Renderer* renderer) {
		if (getBackgroundColor())
			renderer->clear(getBackgroundColor());

		Layout::onRender(renderer);
	}

	uint32_t Application::getPeripheralsDeltaTime() const {
		return _peripheralsDeltaTime;
	}
}