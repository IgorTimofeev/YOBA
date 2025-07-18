#include <cstdint>
#include <YOBA/UI/textField.h>
#include <YOBA/UI/application.h>
#include <YOBA/main/events/keyEvent.h>

namespace YOBA {
	TextField::~TextField() {
		KeyboardController::hide();
	}

	void TextField::onTick() {
		if (isFocused()) {
			if (isCaptured()) {
				if (system::getTime() >= _continuousScrollTime) {
					applyContinuousScroll();
				}
			}
			else {
				if (system::getTime() >= _cursorBlinkTime) {
					setCursorBlinkStateAndTime(!_cursorBlinkState);
					invalidateRender();
				}
			}
		}
	}

	void TextField::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto focused = isFocused();

		// Background
		const auto backgroundColor = Color::select(focused, _defaultBackgroundColor, _focusedBackgroundColor);

		if (backgroundColor) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				backgroundColor
			);
		}

		// Border
		const auto borderColor = Color::select(focused, _defaultBorderColor, _focusedBorderColor);

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

		const auto fontHeight = font->getHeight(getFontScale());
		const auto& text = getText();

		// Placeholder
		if (text.empty() && !isFocused()) {
			if (_placeholderColor) {
				const auto& placeholder = getPlaceholder();

				if (!placeholder.empty()) {
					renderer->renderString(
						Point(
							bounds.getX() + _textMargin,
							bounds.getYCenter() - fontHeight / 2
						),
						font,
						_placeholderColor,
						placeholder,
						getFontScale()
					);
				}
			}
		}
		// Text & cursor
		else {
			auto textPosition = Point(
				bounds.getX() + _textMargin - _scrollPosition,
				bounds.getYCenter() - fontHeight / 2
			);

			auto blinkX = textPosition.getX();

			// Text
			if (!text.empty()) {
				const auto textColor = Color::select(focused, _defaultTextColor, _focusedTextColor);

				if (!textColor)
					return;

				const auto oldViewport = renderer->pushViewport(Bounds(
					bounds.getX() + _textMargin,
					bounds.getY(),
					bounds.getWidth() - _textMargin * 2,
					bounds.getHeight()
				));

				for (size_t charIndex = 0; charIndex < text.length(); charIndex++) {
					const wchar_t ch = _mask ? _mask : text[charIndex];

					renderer->renderChar(
						textPosition,
						font,
						textColor,
						ch,
						getFontScale()
					);

					if (charIndex == _cursorPosition)
						blinkX = textPosition.getX();

					textPosition.setX(textPosition.getX() + font->getWidth(ch, getFontScale()));
				}

				renderer->popViewport(oldViewport);
			}

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
	}

	void TextField::onEvent(Event* event) {
		if (event->getTypeID() == PointerDownEvent::typeID) {
			setCaptured(true);
			setFocused(true);

			_lastTouchX = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getX();

			applyContinuousScroll();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			_lastTouchX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();

			applyContinuousScroll();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			event->setHandled(true);
		}
		else if (event->getTypeID() == KeyUpEvent::typeID) {
			const auto keyUpEvent = reinterpret_cast<KeyUpEvent*>(event);

			switch (keyUpEvent->getKey()) {
				case Key::enter: {
					setFocused(false);
					break;
				}
				case Key::backspace: {
					backspace();
					break;
				}
				default: {
					const auto text = keyUpEvent->getText();

					if (text.has_value()) {
						onInput(keyUpEvent->getKey(), text.value());
						input(keyUpEvent->getKey(), text.value());

						insert(text.value());
					}

					break;
				}
			}

			keyUpEvent->setHandled(true);
		}
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

		const auto& computeCursorPositionFor = [this, &boundsXWithoutMargin, font, &cursorPosition, &cursorX, &text](const int32_t targetX) {
			cursorX = boundsXWithoutMargin - _scrollPosition;
			cursorPosition = 0;

			for (size_t i = 0; i < text.length(); i++) {
				if (cursorX < targetX) {
					cursorPosition++;
					cursorX += font->getWidth(_mask ? _mask : text[i]);
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
					const auto previousCharWidth = font->getWidth(_mask ? _mask : text[cursorPosition], getFontScale());

					_scrollPosition = cursorX > previousCharWidth ? cursorX - previousCharWidth : 0;
				}
			}

			setCursorPosition(cursorPosition);
		}
		else if (_lastTouchX > boundsX2WithoutMargin) {
			computeCursorPositionFor(boundsX2WithoutMargin);

			if (cursorPosition < text.length()) {
				const int32_t pizda = cursorX + font->getWidth(_mask ? _mask : text[cursorPosition], getFontScale()) - boundsWidthWithoutMargin;

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

	void TextField::insert(const std::wstring_view value) {
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
		if (cursorPosition == 0)
			return;

		// End
		if (cursorPosition == text.length()) {
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

	void TextField::setCursorPosition(const size_t value) {
		_cursorPosition = value;

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

	void TextField::setCursorBlinkInterval(const uint32_t value) {
		_cursorBlinkInterval = value;
	}

	const Color* TextField::getCursorColor() const {
		return _cursorColor;
	}

	void TextField::setCursorColor(const Color* cursorColor) {
		_cursorColor = cursorColor;
	}

	const Color* TextField::getDefaultBorderColor() const {
		return _defaultBorderColor;
	}

	void TextField::setDefaultBorderColor(const Color* value) {
		_defaultBorderColor = value;
	}

	const Color* TextField::getDefaultBackgroundColor() const {
		return _defaultBackgroundColor;
	}

	void TextField::setDefaultBackgroundColor(const Color* value) {
		_defaultBackgroundColor = value;
	}

	const Color* TextField::getDefaultTextColor() const {
		return _defaultTextColor;
	}

	void TextField::setDefaultTextColor(const Color* value) {
		_defaultTextColor = value;
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
		Control::onFocusChanged();

		setCursorBlinkStateAndTime(isFocused());

		if (isFocused()) {
			KeyboardController::show();
		}
		else {
			KeyboardController::hide();
		}

		invalidateRender();
	}

	void TextField::onTextChanged() {
		TextElement::onTextChanged();

		textChanged();
	}

	void TextField::setCursorBlinkStateAndTime(const bool value) {
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

	void TextField::onInput(Key keyCode, std::wstring_view text) {

	}

	const Color* TextField::getFocusedBorderColor() const {
		return _focusedBorderColor;
	}

	void TextField::setFocusedBorderColor(const Color* value) {
		_focusedBorderColor = value;
	}

	const Color* TextField::getFocusedBackgroundColor() const {
		return _focusedBackgroundColor;
	}

	void TextField::setFocusedBackgroundColor(const Color* value) {
		_focusedBackgroundColor = value;
	}

	const Color* TextField::getFocusedTextColor() const {
		return _focusedTextColor;
	}

	void TextField::setFocusedTextColor(const Color* value) {
		_focusedTextColor = value;
	}

	const Color* TextField::getPlaceholderColor() const {
		return _placeholderColor;
	}

	void TextField::setPlaceholderColor(const Color* value) {
		_placeholderColor = value;
	}

	wchar_t TextField::getMask() const {
		return _mask;
	}

	void TextField::setMask(const wchar_t mask) {
		_mask = mask;
	}

	std::wstring_view TextField::getPlaceholder() const {
		return _placeholder;
	}

	void TextField::setPlaceholder(const std::wstring_view& value) {
		_placeholder = value;

		invalidate();
	}
}
