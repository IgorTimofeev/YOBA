#include "application.h"

#include <esp_log.h>

#include <YOBA/UI/element.h>
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

	void Application::setup(RenderTarget* renderTarget, Renderer* renderer, HID* hid) {
		setup(renderTarget, renderer);

		hid->setup();
		addHID(hid);
	}

	void Application::invalidateMeasure() {
		_measureInvalidated = true;
	}

	void Application::invalidateRender() {
		_renderInvalidated = true;

//		esp_backtrace_print(10);
	}

	void Application::invalidate() {
		invalidateMeasure();
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

		// Handling input from HIDs like touchscreens, rotary encoders, etc.
		auto time = system::getTime();

		for (const auto hid : _HIDs)
			hid->tick();

		_peripheralsDeltaTime = system::getTime() - time;

		// Handling tick for children
		time = system::getTime();

		onTick();

		// Running scheduled tasks
		if (!_scheduledOnTickTasks.empty()) {
			for (size_t i = 0; i < _scheduledOnTickTasks.size(); i++)
				_scheduledOnTickTasks[i]();

			_scheduledOnTickTasks.clear();
		}

		// Playing animations
		animationsTick();

		_tickDeltaTime = system::getTime() - time;
	}

	void Application::render() {
		uint32_t time;

		// Measuring children size
		if (_measureInvalidated) {
			time = system::getTime();

			measure(getSize());

			_layoutDeltaTime = system::getTime() - time;

			_measureInvalidated = false;
		}

		// Render pass
		if (_renderInvalidated) {
			time = system::getTime();

			// Rendering children
			Layout::render(_renderer, getBounds());

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
		if (element == _capturedElement)
			return;

		const auto previouslyCapturedElement = _capturedElement;
		_capturedElement = element;

		if (previouslyCapturedElement)
			previouslyCapturedElement->onCaptureChanged();

		if (_capturedElement)
			_capturedElement->onCaptureChanged();

		invalidate();
	}

	Element* Application::getFocusedElement() const {
		return _focusedElement;
	}

	void Application::setFocusedElement(Element* element) {
		if (element == _focusedElement)
			return;

		const auto previouslyFocusedElement = _focusedElement;
		_focusedElement = element;

		if (previouslyFocusedElement)
			previouslyFocusedElement->onFocusChanged();

		if (_focusedElement)
			_focusedElement->onFocusChanged();

		invalidate();
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

	void Application::addHID(HID* hid) {
		_HIDs.push_back(hid);
	}

	void Application::scheduleOnTick(const std::function<void()>& task) {
		_scheduledOnTickTasks.push_back(task);
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
