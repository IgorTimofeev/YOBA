#include <YOBA/UI/Application.hpp>
#include <YOBA/UI/Element.hpp>
#include <YOBA/UI/Animations/Animation.hpp>

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

		// Root element size should match rendering target size
		setSize(_renderer->getTarget()->getSize());

		// Resetting clipping just in case if some UI element broke it
		_renderer->resetClip();

		// Handling input from HIDs like touchscreens, rotary encoders, etc.
		for (const auto hid : _HIDs)
			hid->tick();

		// Handling onTick() of children
		onTick();

		// Invoking functions that were scheduled to be invoked later in UI thread
		if (!_functionsToInvokeLater.empty()) {
			for (const auto& task : _functionsToInvokeLater)
				task();

			_functionsToInvokeLater.clear();
		}

		// Handling animations
		if (!_animations.empty()) {
			Animation* animation;

			for (size_t i = 0; i < _animations.size(); i++) {
				animation = _animations[i];

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
	}

	void Application::updateLayout() {
		if (!_layoutInvalidated)
			return;

		// Measuring size of children
		measure(getSize());

		// Arranging children & computing their bounds
		arrange(Rectangle(getSize()));

		_layoutInvalidated = false;
	}

	void Application::render() {
		if (!_renderInvalidated)
			return;

		// Rendering children
		Layout::render(_renderer, getLayoutBounds());

		// Flushing screen buffer
		_renderer->flush();

		_renderInvalidated = false;
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

	bool Application::isLayoutInvalidated() const {
		return _layoutInvalidated;
	}

	bool Application::isRenderInvalidated() const {
		return _renderInvalidated;
	}

	bool Application::isInvalidated() const {
		return _layoutInvalidated || _renderInvalidated;
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

	void Application::onRender(Renderer* renderer, const Rectangle& bounds) {
		if (getBackgroundColor())
			renderer->clear(getBackgroundColor());

		Layout::onRender(renderer, bounds);
	}
}
