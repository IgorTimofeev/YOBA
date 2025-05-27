#include "button.h"

#include <esp_log.h>

namespace YOBA {
	void Button::onTouchDown(TouchDownEvent* event) {
		ESP_LOGI("Button", "onTouchDown");

		_wasDown = true;
		_previousIsActive = isActive();
		setActive(true);

		event->setHandled(true);
	}

	void Button::onTouchUp(TouchUpEvent* event) {
		ESP_LOGI("Button", "onTouchUp");

		if (!_wasDown)
			return;

		if (_isToggle) {
			setActive(!isActive());
		}
		else {
			setActive(false);
		}

		callOnClick();

		_wasDown = false;

		event->setHandled(true);
	}

	void Button::onTouchOverChanged() {
		ESP_LOGI("Button", "onTouchOverChanged: %d", isTouchOver());

		if (!_wasDown || isTouchOver())
			return;

		if (_isToggle) {
			setActive(!_previousIsActive);
		}
		else {
			setActive(false);
		}

		_wasDown = false;
	}

	void Button::onRender(Renderer* renderer, const Bounds& bounds) {
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

		Element::onRender(renderer, bounds);
	}

	void Button::onIsActiveChanged() {
		ActiveElement::onIsActiveChanged();

		isActiveChanged();
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
