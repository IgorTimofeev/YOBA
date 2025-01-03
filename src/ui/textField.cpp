#include <sstream>
#include <cstdint>
#include "textField.h"

#include "../../../../lib/YOBA/src/ui/keyboard.h"
#include "../../../../lib/YOBA/src/ui/application.h"

namespace yoba {
	void TextField::tick() {
		Element::tick();

		if (!isFocused() || isCaptured() || millis() < _cursorBlinkTime)
			return;

		setCursorBlinkStateAndTime(!_cursorBlinkState);
	}

	void TextField::onRender(ScreenBuffer* screenBuffer) {
		auto& bounds = getBounds();

		const auto focused = isFocused();

		// Background
		const auto primaryColor = selectColor(focused, _primaryColor, _focusedPrimaryColor, screenBuffer->getPrimaryColor());

		if (primaryColor) {
			screenBuffer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				primaryColor
			);
		}

		// Border
		const auto borderColor = selectColor(focused, _borderColor, _focusedBorderColor, nullptr);

		if (borderColor) {
			screenBuffer->renderRectangle(
				bounds,
				getCornerRadius(),
				borderColor
			);
		}

		// Text
		const auto font = getFontOrDefault();

		if (!font)
			return;

		const auto secondaryColor = selectColor(focused, _secondaryColor, _focusedSecondaryColor, screenBuffer->getSecondaryColor());

		if (!secondaryColor)
			return;

		const auto& oldViewport = screenBuffer->getViewport();

		screenBuffer->setViewport(Bounds(
			bounds.getX() + _textMargin,
			bounds.getY(),
			bounds.getWidth() - _textMargin * 2,
			bounds.getHeight()
		));

		const auto& text = getText();

		auto textPosition = Point(
			_scrollValue + bounds.getX() + _textMargin,
			bounds.getYCenter() - font->getHeight() / 2
		);

		auto blinkX = textPosition.getX();

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++) {
			screenBuffer->renderChar(
				textPosition,
				font,
				secondaryColor,
				text[charIndex]
			);

			if (charIndex == _cursorPosition)
				blinkX = textPosition.getX();

			textPosition.setX(textPosition.getX() + font->getCharWidth(text[charIndex]));
		}

		// Cursor
		if (_cursorBlinkState) {
			// End of text
			if (_cursorPosition == text.length())
				blinkX = textPosition.getX();

			screenBuffer->renderFilledRectangle(
				Bounds(
					blinkX,
					textPosition.getY() + font->getHeight() / 2 - _cursorSize.getHeight() / 2,
					_cursorSize.getWidth(),
					_cursorSize.getHeight()
				),
				_cursorColor ? _cursorColor : screenBuffer->getSecondaryColor()
			);
		}

		screenBuffer->setViewport(oldViewport);
	}

	void TextField::onEvent(InputEvent& event) {
		const auto isTouchDown = event.getTypeID() == TouchDownEvent::typeID;
		const auto isTouchUp = event.getTypeID() == TouchUpEvent::typeID;
		const auto isTouchDrag = event.getTypeID() == TouchDragEvent::typeID;

		if (!(isTouchDown || isTouchUp || isTouchDrag))
			return;

		if (isTouchDown) {
			setCaptured(true);

			if (!isFocused()) {
				setFocused(true);
			}
		}
		else if (isTouchUp) {
			setCaptured(false);
			return;
		}

		const auto font = getFontOrDefault();

		if (font) {
			auto touchEvent = (TouchEvent&) event;
			const auto& bounds = getBounds();
			auto touchX = touchEvent.getPosition().getX();

			if (touchX >= bounds.getX() && touchX <= bounds.getX2()) {
				const auto text = getText();

				auto textX = _scrollValue + bounds.getX() + _textMargin;
				size_t cursorPosition = 0;

				for (size_t i = 0; i < text.length(); i++) {
					if (touchX > textX) {
						cursorPosition++;
						textX += font->getCharWidth(text[i]);
					}
					else {
						break;
					}
				}

				setCursorPosition(cursorPosition);
			}
		}
		else {
			setCursorToEnd();
		}

		event.setHandled(true);
	}

	void TextField::insert(const std::wstring_view& value) {
		const auto text = getText();
		const auto cursorPosition = getCursorPosition();

		// ABCDE
		// -----

		// Start
		if (cursorPosition == 0) {
			if (text.length() > 0) {
				setText(std::wstring(value) + std::wstring(text));
			}
			else {
				setText(value);
			}
		}
		// End
		else if (cursorPosition == text.length()) {
			setText(std::wstring(text) + std::wstring(value));
		}
		// Middle
		else {
			setText(std::wstring(text.substr(0, cursorPosition)) + std::wstring(value) + std::wstring(text.substr(cursorPosition)));
		}

		setCursorPosition(getCursorPosition() + value.length());
	}

	void TextField::backspace() {
		const auto text = getText();
		const auto cursorPosition = getCursorPosition();

		// Start
		if (cursorPosition == 0) {
			return;
		}
		// End
		else if (cursorPosition == text.length()) {
			if (text.length() > 0) {
				setText(text.substr(0, text.length() - 1));
			}
		}
		// 1
		else if (cursorPosition == 1) {
			if (text.length() > 1) {
				setText(text.substr(1));
			}
			else if (text.length() > 0) {
				setText(std::wstring());
			}
		}
		// Middle
		else {
			setText(std::wstring(text.substr(0, cursorPosition - 1)) + std::wstring(text.substr(cursorPosition)));
		}

		setCursorPosition(getCursorPosition() - 1);
	}

	size_t TextField::getCursorPosition() const {
		return _cursorPosition;
	}

	void TextField::setCursorPosition(size_t cursorPosition) {
		_cursorPosition = cursorPosition;

		const auto textLength = getText().length();

		if (_cursorPosition > textLength)
			_cursorPosition = textLength;

		if (isFocused())
			setCursorBlinkStateAndTime(true);

		invalidateRender();
	}

	uint32_t TextField::getCursorBlinkInterval() const {
		return _cursorBlinkInterval;
	}

	void TextField::setCursorBlinkInterval(uint32_t cursorBlinkInterval) {
		_cursorBlinkInterval = cursorBlinkInterval;
	}

	const Color* TextField::getCursorColor() const {
		return _cursorColor;
	}

	void TextField::setCursorColor(const Color* cursorColor) {
		_cursorColor = cursorColor;
	}

	const Size& TextField::getCursorSize() const {
		return _cursorSize;
	}

	void TextField::setCursorSize(const Size& cursorSize) {
		_cursorSize = cursorSize;

		invalidateRender();
	}

	void TextField::setCursorToStart() {
		setCursorPosition(0);
	}

	void TextField::setCursorToEnd() {
		setCursorPosition(getText().length());
	}

	void TextField::onFocusChanged() {
		Element::onFocusChanged();

		setCursorBlinkStateAndTime(isFocused());

		if (isFocused()) {
			showKeyboard();
		}
		else {
			ApplicationKeyboardController::hide();
		}

		invalidateRender();
	}

	void TextField::setCursorBlinkStateAndTime(bool value) {
		_cursorBlinkState = value;
		_cursorBlinkTime = millis() + _cursorBlinkInterval;
	}

	const uint16_t& TextField::getTextMargin() const {
		return _textMargin;
	}

	void TextField::setTextMargin(const uint16_t& textMargin) {
		_textMargin = textMargin;

		invalidateRender();
	}

	void TextField::showKeyboard() {
		auto keyboard = ApplicationKeyboardController::show(getApplication());

		if (_keyboardConfigurator.has_value())
			_keyboardConfigurator.value()(keyboard);

		keyboard->getOnKeyPressedChanged() += [this](KeyCode code, bool pressed) {
			if (pressed)
				return;

			switch (code) {
				case KeyCode::Enter: {
					setFocused(false);
					break;
				}
				case KeyCode::Backspace: {
					backspace();
					break;
				}
				default:
					break;
			}
		};

		keyboard->getOnInput() += [this](KeyCode code, const std::wstring_view& text) {
			insert(text);
		};
	}

	const std::optional<std::function<void(Keyboard*)>>& TextField::getKeyboardConfigurator() const {
		return _keyboardConfigurator;
	}

	void TextField::setKeyboardConfigurator(const std::optional<std::function<void(Keyboard*)>>& keyboardConfigurator) {
		_keyboardConfigurator = keyboardConfigurator;
	}

	const Color* TextField::getFocusedBorderColor() const {
		return _focusedBorderColor;
	}

	void TextField::setFocusedBorderColor(const Color* focusedBorderColor) {
		_focusedBorderColor = focusedBorderColor;
	}

	const Color* TextField::getFocusedPrimaryColor() const {
		return _focusedPrimaryColor;
	}

	void TextField::setFocusedPrimaryColor(const Color* focusedPrimaryColor) {
		_focusedPrimaryColor = focusedPrimaryColor;
	}

	const Color* TextField::getFocusedSecondaryColor() const {
		return _focusedSecondaryColor;
	}

	void TextField::setFocusedSecondaryColor(const Color* focusedSecondaryColor) {
		_focusedSecondaryColor = focusedSecondaryColor;
	}
}