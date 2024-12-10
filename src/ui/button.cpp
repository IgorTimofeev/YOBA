#include "button.h"

namespace yoba {
	void Button::onRender(ScreenBuffer* screenBuffer) {
		const auto& bounds = getBounds();

		// Primary color
		if (getPrimaryColor()) {
			screenBuffer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				isPressed() && getPressedPrimaryColor()
				? getPressedPrimaryColor()
				: getPrimaryColor()
			);
		}

		// Text
		if (getText()) {
			const auto font = getFontOrDefault();

			if (font) {
				auto textSize = font->getSize(getText());

				screenBuffer->renderText(
					Point(
						bounds.getXCenter() - textSize.getXCenter(),
						bounds.getYCenter() - textSize.getYCenter()
					),
					font,
					isPressed() && getPressedSecondaryColor()
					? getPressedSecondaryColor()
					: getSecondaryColor(),
					getText()
				);
			}
		}
	}

	void Button::onEvent(InputEvent& event) {
		if (event.getTypeID() == TouchDownEvent::typeID) {
			setCaptured(true);
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
		_pressed = value;
	}

	bool Button::isToggle() const {
		return _toggle;
	}

	void Button::setToggle(bool value) {
		_toggle = value;
	}

	const Color *Button::getPressedPrimaryColor() const {
		return _pressedPrimaryColor;
	}

	void Button::setPressedPrimaryColor(const Color* value) {
		_pressedPrimaryColor = value;
	}

	const Color *Button::getPressedSecondaryColor() const {
		return _pressedSecondaryColor;
	}

	void Button::setPressedForeground(const Color* value) {
		_pressedSecondaryColor = value;
	}
}