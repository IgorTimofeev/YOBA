#include "button.h"

namespace YOBA {
	void Button::onRender(Renderer* renderer, const Bounds& bounds) {
		// Primary color
		auto color = Color::select(isChecked(), _defaultBackgroundColor, _pressedBackgroundColor);

		if (color) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				color
			);
		}

		// Border
		color = Color::select(isChecked(), _defaultBorderColor, _pressedBorderColor);

		if (color) {
			renderer->renderRectangle(
				bounds,
				getCornerRadius(),
				color
			);
		}

		// Text
		const auto font = getFont();

		if (font) {
			color = Color::select(isChecked(), _defaultTextColor, _pressedTextColor);

			if (color) {
				renderer->renderString(
					Point(
						bounds.getXCenter() - font->getWidth(getText(), getFontScale()) / 2 + getContentMargin().getLeft() - getContentMargin().getRight(),
						bounds.getYCenter() - font->getHeight(getFontScale()) / 2 + getContentMargin().getTop() - getContentMargin().getBottom()
					),
					font,
					color,
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

			switch (_checkMode) {
				case ButtonCheckMode::normal: {
					setChecked(true);

					break;
				}
				default: {
					break;
				}
			}

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			const auto touchUpEvent = static_cast<TouchUpEvent*>(event);
			const auto intersects = getBounds().intersects(touchUpEvent->getPosition());

			switch (_checkMode) {
				case ButtonCheckMode::normal: {
					setChecked(false);

					if (intersects)
						callOnClick();

					break;
				}
				case ButtonCheckMode::toggle: {
					if (intersects) {
						setChecked(!isChecked());
						callOnClick();
					}

					break;
				}
				default: {
					if (intersects)
						callOnClick();

					break;
				}
			}

			setCaptured(false);

			event->setHandled(true);
		}
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

	const Color* Button::getDefaultBorderColor() const {
		return _defaultBorderColor;
	}

	void Button::setDefaultBorderColor(const Color* defaultBorderColor) {
		_defaultBorderColor = defaultBorderColor;
	}

	const Color* Button::getPressedBorderColor() const {
		return _pressedBorderColor;
	}

	void Button::setPressedBorderColor(const Color* pressedBorderColor) {
		_pressedBorderColor = pressedBorderColor;
	}

	void Button::onClick() {

	}

	void Button::callOnClick() {
		onClick();

		click();
	}

	const Margin& Button::getContentMargin() const {
		return _contentMargin;
	}

	void Button::setContentMargin(const Margin& contentMargin) {
		_contentMargin = contentMargin;

		invalidate();
	}

	ButtonCheckMode Button::getCheckMode() const {
		return _checkMode;
	}

	void Button::setCheckMode(ButtonCheckMode value) {
		_checkMode = value;
	}
}