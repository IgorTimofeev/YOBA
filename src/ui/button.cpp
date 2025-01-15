#include "button.h"

namespace yoba::ui {
	void Button::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		// Primary color
		auto primaryColor = getPrimaryColor();

		if (!primaryColor)
			primaryColor = renderer->getPrimaryColor();

		if (primaryColor) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				isPressed() && getPressedPrimaryColor()
				? getPressedPrimaryColor()
				: primaryColor
			);
		}

		// Text
		const auto font = getFontOrDefault();

		if (font) {
			auto secondaryColor =
				isPressed() && getPressedSecondaryColor()
				? getPressedSecondaryColor()
				: getSecondaryColor();

			if (!secondaryColor)
				secondaryColor = renderer->getSecondaryColor();

			renderer->renderText(
				Point(
					bounds.getXCenter() - font->getWidth(getText()) / 2,
					bounds.getYCenter() - font->getHeight() / 2
				),
				font,
				secondaryColor,
				getText()
			);
		}

		Element::onRender(renderer);
	}

	void Button::onEvent(InputEvent& event) {
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
		_onPressedChanged();
	}

	bool Button::isToggle() const {
		return _toggle;
	}

	void Button::setToggle(bool value) {
		_toggle = value;
	}

	const Color* Button::getPressedPrimaryColor() const {
		return _pressedPrimaryColor;
	}

	void Button::setPressedPrimaryColor(const Color* value) {
		_pressedPrimaryColor = value;
	}

	const Color* Button::getPressedSecondaryColor() const {
		return _pressedSecondaryColor;
	}

	void Button::setPressedSecondaryColor(const Color* value) {
		_pressedSecondaryColor = value;
	}

	Callback<>& Button::getOnPressedChanged() {
		return _onPressedChanged;
	}

	void Button::setOnClick(const Callback<>& onClick) {
		_onPressedChanged = onClick;
	}

	void Button::onPressedChanged() {

	}
}