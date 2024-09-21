#include "button.h"

namespace yoba {
	void Button::onRender(ScreenBuffer *screenBuffer) {
		const auto& bounds = getBounds();

		// Background
		if (getBackgroundColor()) {
			screenBuffer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				isPressed() && getPressedBackground()
				? getPressedBackground()
				: getBackgroundColor()
			);
		}

		// Text
		if (getText()) {
			const auto font = getFontOrDefault();

			if (font) {
				auto textSize = font->getSize(getText());

				screenBuffer->renderText(
					Point(
						bounds.getXCenter() - textSize.getWidth() / 2,
						bounds.getYCenter() - textSize.getHeight() / 2
					),
					font,
					isPressed() && getPressedForeground()
					? getPressedForeground()
					: getForegroundColor(),
					getText()
				);
			}
		}
	}

	void Button::onEvent(InputEvent &event) {
		switch (event.getType()) {
			case EventType::TouchDown: {
				setCaptured(true);
				setPressed(!isToggle() || !isPressed());

				event.setHandled(true);

				break;
			}
			case EventType::TouchUp: {
				setCaptured(false);

				if (!isToggle())
					setPressed(false);

				event.setHandled(true);

				break;
			}
			default:
				break;
		}
	}

	bool Button::isPressed() const {
		return _pressed;
	}

	void Button::setPressed(bool pressed) {
		_pressed = pressed;
	}

	bool Button::isToggle() const {
		return _toggle;
	}

	void Button::setToggle(bool toggle) {
		_toggle = toggle;
	}

	const Color *Button::getPressedBackground() const {
		return _pressedBackground;
	}

	void Button::setPressedBackground(const Color *pressedBackground) {
		_pressedBackground = pressedBackground;
	}

	const Color *Button::getPressedForeground() const {
		return _pressedForeground;
	}

	void Button::setPressedForeground(const Color *pressedForeground) {
		_pressedForeground = pressedForeground;
	}
}