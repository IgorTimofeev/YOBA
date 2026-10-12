#include <cstdint>

#include <YOBA/UI/Keyboard/Overlay.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	KeyboardOverlay::KeyboardOverlay() {
		*this += &_contentLayout;

		*this += &_keyboard;
		setAutoSize(&_keyboard);
	}

	Keyboard* KeyboardOverlay::getKeyboard() {
		return &_keyboard;
	}

	Layout* KeyboardOverlay::getTargetLayout() const {
		return _targetLayout;
	}

	void KeyboardOverlay::setTargetLayout(Layout* value) {
		_targetLayout = value;
	}

	Layout* KeyboardOverlay::getTargetLayoutOrApplication() const {
		return _targetLayout ? _targetLayout : Application::getCurrent();
	}

	bool KeyboardOverlay::isShown() const {
		return _isShown;
	}

	void KeyboardOverlay::show() {
		if (_isShown)
			return;

		_isShown = true;

		const auto targetLayout = getTargetLayoutOrApplication();

		setSize(targetLayout->getLayoutBounds().getSize());

		// Moving children from target to temporary layout
		targetLayout->moveChildrenTo(&_contentLayout);

		*targetLayout += this;

		if (_onStateChanged)
			_onStateChanged();
	}

	void KeyboardOverlay::hide() {
		if (!_isShown)
			return;

		_isShown = false;

		const auto targetLayout = getTargetLayoutOrApplication();

		targetLayout->removeChildren();
		_contentLayout.moveChildrenTo(targetLayout);

		if (_onStateChanged)
			_onStateChanged();
	}

	void KeyboardOverlay::setOnIsShownChanged(const std::function<void()>& callback) {
		_onStateChanged = callback;
	}

	void KeyboardOverlay::onEventBeforeChildren(Event* event) {
		RelativeStackLayout::onEventBeforeChildren(event);

		if (const auto keyEvent = event->as<KeyDownEvent>()) {
			if (keyEvent->getKey() == Key::enter) {
				hide();
			}
		}
	}
}
