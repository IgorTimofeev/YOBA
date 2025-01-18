#include "button.h"

namespace yoba::ui {
	void Button::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		// Primary color
		auto backgroundColor = Color::select(isPressed(), _defaultBackgroundColor, _pressedBackgroundColor);

		if (backgroundColor) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				backgroundColor
			);
		}

		// Text
		const auto font = getFont();

		if (font) {
			auto textColor = Color::select(isPressed(), _defaultTextColor, _pressedTextColor);

			if (textColor) {
				renderer->renderString(
					Point(
						bounds.getXCenter() - font->getWidth(getText()) / 2,
						bounds.getYCenter() - font->getHeight() / 2
					),
					font,
					textColor,
					getText(),
					getFontScale()
				);
			}
		}

		Element::onRender(renderer);
	}

	void Button::onEvent(Event& event) {
		if (event.getTypeID() == TouchDownEvent::typeID) {
			setCaptured(true);
			setFocused(true);

			setPressed(!isToggle() || !isPressed());

			event.setHandled(true);
		}
		else if (event.getTypeID() == TouchUpEvent::typeID) {
			setCaptured(false);

			if (!isToggle())
				setPressed(false);

			event.setHandled(true);
		}
	}

	bool Button::isPressed() const {
		return _pressed;
	}

	void Button::setPressed(bool value) {
		if (value == _pressed)
			return;

		_pressed = value;

		invalidate();

		onPressedChanged();
		pressedChanged();
	}

	bool Button::isToggle() const {
		return _toggle;
	}

	void Button::setToggle(bool value) {
		_toggle = value;
	}

	const Color* Button::getPressedBackgroundColor() const {
		return _pressedBackgroundColor;
	}

	void Button::setPressedBackgroundColor(const Color* value) {
		_pressedBackgroundColor = value;
	}

	const Color* Button::getPressedTextColor() const {
		return _pressedTextColor;
	}

	void Button::setPressedTextColor(const Color* value) {
		_pressedTextColor = value;
	}

	void Button::onPressedChanged() {

	}

	const Color* Button::getDefaultBackgroundColor() const {
		return _defaultBackgroundColor;
	}

	void Button::setDefaultBackgroundColor(const Color* value) {
		_defaultBackgroundColor = value;
	}

	const Color* Button::getDefaultTextColor() const {
		return _defaultTextColor;
	}

	void Button::setDefaultTextColor(const Color* value) {
		_defaultTextColor = value;
	}
}