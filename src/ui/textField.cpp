#include <cstdint>
#include "textField.h"

#include "application.h"

namespace yoba::ui {
	void TextField::onTick() {
		Element::onTick();

		if (isFocused()) {
			if (isCaptured()) {
				if (system::getTime() >= _continuousScrollTime) {
					applyContinuousScroll();
				}
			}
			else {
				if (system::getTime() >= _cursorBlinkTime) {
					setCursorBlinkStateAndTime(!_cursorBlinkState);
				}
			}
		}
	}

	void TextField::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto focused = isFocused();

		// Background
		const auto primaryColor = Color::select(focused, getPrimaryColor(), _focusedPrimaryColor);

		if (primaryColor) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				primaryColor
			);
		}

		// Border
		const auto borderColor = Color::select(focused, getBorderColor(), _focusedBorderColor);

		if (borderColor) {
			renderer->renderRectangle(
				bounds,
				getCornerRadius(),
				borderColor
			);
		}

		// Text
		const auto font = getFont();

		if (!font)
			return;

		const auto secondaryColor = Color::select(focused, getSecondaryColor(), _focusedSecondaryColor);

		if (!secondaryColor)
			return;

		const auto oldViewport = renderer->pushViewport(Bounds(
			bounds.getX() + _textMargin,
			bounds.getY(),
			bounds.getWidth() - _textMargin * 2,
			bounds.getHeight()
		));

		const auto& text = getText();
		const auto fontHeight = font->getHeight(getFontScale());

		auto textPosition = Point(
			bounds.getX() + _textMargin - _scrollPosition,
			bounds.getYCenter() - fontHeight / 2
		);

		auto blinkX = textPosition.getX();

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++) {
			renderer->renderChar(
				textPosition,
				font,
				secondaryColor,
				text[charIndex],
				getFontScale()
			);

			if (charIndex == _cursorPosition)
				blinkX = textPosition.getX();

			textPosition.setX(textPosition.getX() + font->getCharWidth(text[charIndex], getFontScale()));
		}

		renderer->popViewport(oldViewport);

		// Cursor
		if (_cursorBlinkState && _cursorColor) {
			// End of text
			if (_cursorPosition == text.length())
				blinkX = textPosition.getX();

			renderer->renderFilledRectangle(
				Bounds(
					blinkX,
					textPosition.getY() + fontHeight / 2 - _cursorSize.getHeight() / 2,
					_cursorSize.getWidth(),
					_cursorSize.getHeight()
				),
				_cursorColor
			);
		}
	}

	void TextField::onEvent(Event* event) {
		const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
		const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;
		const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;

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

		_lastTouchX = ((TouchEvent*) event)->getPosition().getX();

		applyContinuousScroll();

		event->setHandled(true);
	}

	void TextField::applyContinuousScroll() {
		const auto font = getFont();

		if (!font)
			return;

		const auto text = getText();
		const auto& bounds = getBounds();
		const int32_t boundsXWithoutMargin = bounds.getX() + _textMargin;
		const uint16_t boundsWidthWithoutMargin = bounds.getWidth() - _textMargin * 2;
		const int32_t boundsX2WithoutMargin = boundsXWithoutMargin + boundsWidthWithoutMargin - 1;

		size_t cursorPosition;
		int32_t cursorX;

		const auto& computeCursorPositionFor = [this, &boundsXWithoutMargin, font, &cursorPosition, &cursorX, &text](int32_t targetX) {
			cursorX = boundsXWithoutMargin - _scrollPosition;
			cursorPosition = 0;

			for (size_t i = 0; i < text.length(); i++) {
				if (cursorX < targetX) {
					cursorPosition++;
					cursorX += font->getCharWidth(text[i]);
				}
				else {
					break;
				}
			}

			// Converting to [0 px; Cursor px]
			cursorX -= (boundsXWithoutMargin - _scrollPosition);
		};

		if (_lastTouchX < boundsXWithoutMargin) {
			computeCursorPositionFor(boundsXWithoutMargin);

			if (cursorPosition > 0) {
				cursorPosition--;

				if (_scrollPosition > 0) {
					const auto previousCharWidth = font->getCharWidth(text[cursorPosition], getFontScale());

					_scrollPosition = cursorX > previousCharWidth ? cursorX - previousCharWidth : 0;
				}
			}

			setCursorPosition(cursorPosition);
		}
		else if (_lastTouchX > boundsX2WithoutMargin) {
			computeCursorPositionFor(boundsX2WithoutMargin);

			if (cursorPosition < text.length()) {
				int32_t pizda = cursorX + font->getCharWidth(text[cursorPosition], getFontScale()) - boundsWidthWithoutMargin;

				_scrollPosition = pizda > 0 ? pizda : 0;

				cursorPosition++;
			}

			setCursorPosition(cursorPosition);
		}
		else {
			computeCursorPositionFor(_lastTouchX);
			setCursorPosition(cursorPosition);
		}

		_continuousScrollTime = system::getTime() + _continuousScrollInterval;
	}

	void TextField::insert(std::wstring_view value) {
		const auto text = getText();
		const auto cursorPosition = getCursorPosition();

		// ABCDE
		// -----

		// Start
		if (cursorPosition == 0) {
			if (text.empty()) {
				setText(value);
			}
			else {
				setText(std::wstring(value) + std::wstring(text));
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
			if (!text.empty()) {
				setText(text.substr(0, text.length() - 1));
			}
		}
		// 1
		else if (cursorPosition == 1) {
			if (text.length() > 1) {
				setText(text.substr(1));
			}
			else if (!text.empty()) {
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

	void TextField::showKeyboard() {
		auto keyboard = ApplicationKeyboardController::show(getApplication());

		if (_keyboardConfigurator.has_value())
			_keyboardConfigurator.value()(keyboard);

		keyboard->keyPressedChanged += [this](KeyCode code, bool pressed) {
			if (pressed)
				return;

			switch (code) {
				case KeyCode::enter: {
					setFocused(false);
					break;
				}
				case KeyCode::backspace: {
					backspace();
					break;
				}
				default:
					break;
			}
		};

		keyboard->input += [this](KeyCode code, std::wstring_view text) {
			insert(text);
		};
	}

	void TextField::setCursorBlinkStateAndTime(bool value) {
		_cursorBlinkState = value;
		_cursorBlinkTime = system::getTime() + _cursorBlinkInterval;
	}

	const uint16_t& TextField::getTextMargin() const {
		return _textMargin;
	}

	void TextField::setTextMargin(const uint16_t& textMargin) {
		_textMargin = textMargin;

		invalidateRender();
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