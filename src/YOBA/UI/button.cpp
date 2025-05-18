#include "button.h"

namespace YOBA {
	void Button::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		// Primary color
		auto color = Color::select(isActive(), _defaultBackgroundColor, _activeBackgroundColor);

		if (color) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				color
			);
		}

		// Border
		color = Color::select(isActive(), _defaultBorderColor, _activeBorderColor);

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
			color = Color::select(isActive(), _defaultTextColor, _activeTextColor);

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

		Element::onRender(renderer);
	}

	void Button::onEvent(Event* event) {
		if (event->getTypeID() == TouchDownEvent::typeID) {
			setCaptured(true);
			setFocused(true);

			_wasDown = true;
			_previousIsActive = isActive();
			setActive(true);

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			if (!_wasDown)
				return;

			const auto touchUpEvent = static_cast<TouchUpEvent*>(event);
			const auto inBounds = getBounds().intersects(touchUpEvent->getPosition());

			if (isToggle()) {
				if (inBounds) {
					setActive(!isActive());
					callOnClick();
				}
				else {
					setActive(_previousIsActive);
				}
			}
			else {
				setActive(false);

				if (inBounds)
					callOnClick();
			}

			setCaptured(false);
			_wasDown = false;

			event->setHandled(true);
		}
	}

	bool Button::isToggle() const {
		return _isToggle;
	}

	void Button::setToggle(bool value) {
		_isToggle = value;
	}

	const Color* Button::getActiveBackgroundColor() const {
		return _activeBackgroundColor;
	}

	void Button::setActiveBackgroundColor(const Color* value) {
		_activeBackgroundColor = value;
	}

	const Color* Button::getActiveTextColor() const {
		return _activeTextColor;
	}

	void Button::setActiveTextColor(const Color* value) {
		_activeTextColor = value;
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

	const Color* Button::getActiveBorderColor() const {
		return _activeBorderColor;
	}

	void Button::setActiveBorderColor(const Color* pressedBorderColor) {
		_activeBorderColor = pressedBorderColor;
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
}