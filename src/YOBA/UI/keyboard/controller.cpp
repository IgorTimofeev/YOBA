#include <cstdint>

#include <YOBA/UI/keyboard/controller.h>
#include <YOBA/UI/stackLayout.h>
#include <YOBA/UI/application.h>
#include <YOBA/main/math.h>

namespace YOBA {
	KeyboardControllerLayout::KeyboardControllerLayout() {
		*this += &temporaryLayout;

		*this += &keyboard;
		setAutoSize(&keyboard);
	}

	KeyboardControllerLayout* KeyboardController::_controllerLayout = nullptr;
	Layout* KeyboardController::_targetLayout = nullptr;
	std::optional<std::function<void(Keyboard*)>> KeyboardController::_onKeyboardShow = std::nullopt;

	void KeyboardController::show(const uint8_t layoutOptions) {
		if (!_controllerLayout) {
			_controllerLayout = new KeyboardControllerLayout();

			const auto targetLayout = getTargetLayoutOrApplication();

			_controllerLayout->setSize(targetLayout->getBounds().getSize());

			// Moving children from target to temporary layout
			targetLayout->moveChildrenTo(&_controllerLayout->temporaryLayout);

			// Configuring keyboard
			if (_onKeyboardShow.has_value())
				_onKeyboardShow.value()(&_controllerLayout->keyboard);

			*targetLayout += _controllerLayout;
		}

		_controllerLayout->keyboard.setLayout(layoutOptions);
	}

	void KeyboardController::hide() {
		if (!_controllerLayout)
			return;

		const auto targetLayout = getTargetLayoutOrApplication();

		targetLayout->removeChildren();
		_controllerLayout->temporaryLayout.moveChildrenTo(targetLayout);

		delete _controllerLayout;
		_controllerLayout = nullptr;
	}

	bool KeyboardController::isVisible() {
		return _controllerLayout != nullptr;
	}

	void KeyboardController::setOnShow(const std::optional<std::function<void(Keyboard*)>>& value) {
		_onKeyboardShow = value;
	}

	Keyboard* KeyboardController::getKeyboard() {
		return _controllerLayout ? &_controllerLayout->keyboard : nullptr;
	}

	Layout* KeyboardController::getTargetLayout() {
		return _targetLayout;
	}

	void KeyboardController::setTargetLayout(Layout* value) {
		_targetLayout = value;
	}

	Layout* KeyboardController::getTargetLayoutOrApplication() {
		return _targetLayout ? _targetLayout : Application::getCurrent();
	}
}