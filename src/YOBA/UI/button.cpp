#include "button.h"

#include <esp_log.h>

#include "application.h"

namespace YOBA {
	void Button::onEvent(Event& event) {
		if (event.getTypeID() == PointerDownEvent::typeID) {
			// ESP_LOGI("Button", "PointerDownEvent");

			_pointerWasDown = true;
			_previousIsActive = isActive();
			setActive(true);
			setFocused(true);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerUpEvent::typeID) {
			// ESP_LOGI("Button", "PointerUpEvent");
			// ESP_LOGI("Button", "isCaptured(): %d", isCaptured());
			// ESP_LOGI("Button", "isPointerOver(): %d", isPointerOver());
			// ESP_LOGI("Button", "global capt: %p, this %p", Application::getCurrent().getCapturedElement(), this);
			// ESP_LOGI("Button", "_wasDown: %d", _wasDown);

			if (_pointerWasDown) {
				if (_isToggle) {
					setActive(!isActive());
				}
				else {
					setActive(false);
				}

				callOnClick();
				
				_pointerWasDown = false;
			}

			setFocused(false);

			event.setHandled(true);
		}
		else if (PointerEvent::isPointer(event)) {
			event.setHandled(true);
		}
	}

	void Button::onPointerOverChanged() {
		// ESP_LOGI("Button", "onPointerOverChanged(): %d", isPointerOver());

		if (!_pointerWasDown || isPointerOver())
			return;

		if (_isToggle) {
			setActive(_previousIsActive);
		}
		else {
			setActive(false);
		}
	}

	void Button::onRender(Renderer& renderer, const Bounds& bounds) {
		// Primary color
		auto color = Color::select(isActive(), _defaultBackgroundColor, _activeBackgroundColor);

		if (color) {
			renderer.renderFilledRectangle(
				bounds,
				getCornerRadius(),
				*color
			);
		}

		// Border
		color = Color::select(isActive(), _defaultBorderColor, _activeBorderColor);

		if (color) {
			renderer.renderRectangle(
				bounds,
				getCornerRadius(),
				*color
			);
		}

		// Text
		if (!hasFont())
			return;

		const auto& font = getFont();

		color = Color::select(isActive(), _defaultTextColor, _activeTextColor);

		if (color) {
			renderer.renderText(
				Point(
					bounds.getXCenter() - font.getWidth(getText(), getFontScale()) / 2 + getContentMargin().getLeft() - getContentMargin().getRight(),
					bounds.getYCenter() - font.getHeight(getFontScale()) / 2 + getContentMargin().getTop() - getContentMargin().getBottom()
				),
				font,
				*color,
				getText(),
				getFontScale()
			);
		}

		Element::onRender(renderer, bounds);
	}

	bool Button::isToggle() const {
		return _isToggle;
	}

	void Button::setToggle(const bool value) {
		_isToggle = value;
	}

	const Color& Button::getActiveBackgroundColor() const {
		return *_activeBackgroundColor;
	}

	void Button::setActiveBackgroundColor(const Color& value) {
		_activeBackgroundColor = &value;
	}

	const Color& Button::getActiveTextColor() const {
		return *_activeTextColor;
	}

	void Button::setActiveTextColor(const Color& value) {
		_activeTextColor = &value;
	}

	const Color& Button::getDefaultBackgroundColor() const {
		return *_defaultBackgroundColor;
	}

	void Button::setDefaultBackgroundColor(const Color& value) {
		_defaultBackgroundColor = &value;
	}

	const Color& Button::getDefaultTextColor() const {
		return *_defaultTextColor;
	}

	void Button::setDefaultTextColor(const Color& value) {
		_defaultTextColor = &value;
	}

	const Color& Button::getDefaultBorderColor() const {
		return *_defaultBorderColor;
	}

	void Button::setDefaultBorderColor(const Color& defaultBorderColor) {
		_defaultBorderColor = &defaultBorderColor;
	}

	const Color& Button::getActiveBorderColor() const {
		return *_activeBorderColor;
	}

	void Button::setActiveBorderColor(const Color& pressedBorderColor) {
		_activeBorderColor = &pressedBorderColor;
	}

	void Button::onClick() {

	}

	void Button::callOnClick() {
		onClick();

		if (_onClick)
			_onClick();
	}

	const Margin& Button::getContentMargin() const {
		return _contentMargin;
	}

	void Button::setContentMargin(const Margin& contentMargin) {
		_contentMargin = contentMargin;

		invalidate();
	}

	void Button::setOnClick(const std::function<void()>& onClick) {
		_onClick = onClick;
	}
}
