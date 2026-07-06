#include "application.h"

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

	void Application::setup(RenderingTarget* renderingTarget, Renderer* renderer) {
		renderingTarget->setup();
		renderer->setTarget(renderingTarget);
		setRenderer(renderer);
	}

	void Application::setup(RenderingTarget* renderingTarget, Renderer* renderer, HID* hid) {
		setup(renderingTarget, renderer);

		addHID(hid);
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

	void Application::addAnimation(Animation* animation) {
		_animations.push_back(animation);
	}

	void Application::removeAnimation(Animation* animation) {
		const auto iterator = std::ranges::find(_animations, animation);

		if (iterator != _animations.end())
			_animations.erase(iterator);
	}

	void Application::tick() {
		_current = this;

		// Root element size should match render target size
		setSize(_renderer->getTarget()->getSize());

		// Resetting clipping just in case if some UI element broke it
		_renderer->resetClip();

		// Handling input from HIDs like touchscreens, rotary encoders, etc.
		auto time = system::getTimeUs();

		for (const auto hid : _HIDs)
			hid->tick();
		
		_HIDTickDeltaTime = system::getTimeUs() - time;

		// Handling onTick() of children
		time = system::getTimeUs();

		onTick();

		// Running functions that were scheduled to be invoked later in UI thread
		if (!_functionsToInvokeLater.empty()) {
			for (const auto& task : _functionsToInvokeLater)
				task();

			_functionsToInvokeLater.clear();
		}

		// Handling animations
		if (!_animations.empty()) {
			for (size_t i = 0; i < _animations.size(); i++) {
				const auto animation = _animations[i];

				animation->tick();

				switch (animation->getState()) {
					case AnimationState::stopped:
					case AnimationState::completed: {
						_animations.erase(_animations.begin() + i);
						i--;
						break;
					}
					default: break;
				}
			}
		}

		_tickDeltaTime = system::getTimeUs() - time;
	}

	void Application::render() {
		uint64_t time;

		// Layout pass
		if (_layoutInvalidated) {
			time = system::getTimeUs();

			// Measuring size of children
			measure(getSize());

			// Arranging children & computing their bounds
			arrange(Rectangle(getSize()));

			_layoutInvalidated = false;

			_layoutDeltaTime = system::getTimeUs() - time;
		}

		// Render pass
		if (_renderInvalidated) {
			// Rendering children
			time = system::getTimeUs();

			Layout::render(_renderer, getLayoutBounds());

			_renderDeltaTime = system::getTimeUs() - time;

			// Flushing screen buffer
			time = system::getTimeUs();

			_renderer->flush();

			_renderInvalidated = false;

			_flushDeltaTime = system::getTimeUs() - time;
		}
	}

	void Application::pushEvent(Event* event) {
		handleEvent(event, getLayoutBounds(), true);
	}

	Element* Application::getCapturedElement() const {
		return _capturedElement;
	}

	void Application::setCapturedElement(Element* element) {
		// ESP_LOGI("Application", "setCapturedElement(): %p, previously capt %p", element, _capturedElement);

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

	Renderer* Application::getRenderer() const {
		return _renderer;
	}

	void Application::addHID(HID* hid) {
		_HIDs.push_back(hid);
	}

	void Application::invokeLater(const std::function<void()>& func) {
		_functionsToInvokeLater.push_back(func);
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

	void Application::onRender(Renderer* renderer, const Rectangle& bounds) {
		if (getBackgroundColor())
			renderer->clear(getBackgroundColor());

		Layout::onRender(renderer, bounds);
	}

	uint32_t Application::getHIDTickDeltaTime() const {
		return _HIDTickDeltaTime;
	}
}
