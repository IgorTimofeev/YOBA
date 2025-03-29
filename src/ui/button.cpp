#include "button.h"

namespace yoba::ui {
	void Button::onRender(Renderer* renderer, const Bounds& bounds) {
		// Primary color
		auto backgroundColor = Color::select(isChecked(), _defaultBackgroundColor, _pressedBackgroundColor);

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
			auto textColor = Color::select(isChecked(), _defaultTextColor, _pressedTextColor);

			if (textColor) {
				renderer->renderString(
					Point(
						bounds.getXCenter() - font->getWidth(getText(), getFontScale()) / 2,
						bounds.getYCenter() - font->getHeight(getFontScale()) / 2
					),
					font,
					textColor,
					getText(),
					getFontScale()
				);
			}
		}

		Element::onRender(renderer, bounds);
	}

	void Button::onEvent(Event* event) {
		if (event->getTypeID() == TouchDownEvent::typeID) {
			setCaptured(true);
			setFocused(true);

			if (isToggle()) {
				if (isChecked()) {
					setChecked(false);
				}
				else {
					setChecked(true);

					callOnClick();
				}
			}
			else {
				setChecked(true);

				callOnClick();
			}

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			setCaptured(false);

			if (!isToggle())
				setChecked(false);

			event->setHandled(true);
		}
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

	void Button::onClick() {

	}

	void Button::callOnClick() {
		onClick();

		click();
	}
}