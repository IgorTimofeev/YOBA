#include <YOBA/UI/TextField.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/Core/Events/KeyEvent.hpp>
#include <YOBA/Core/Events/ScrollIntoViewEvent.hpp>
#include <YOBA/UI/Keyboard/Controller.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>
#include <YOBA/Core/UTF-8.hpp>

namespace YOBA {
	void TextField::onTick() {
		if (isFocused()) {
			if (isCaptured()) {
				if (system::getTimeUs() >= _continuousScrollTime) {
					applyContinuousScroll();
				}
			}
			else {
				if (system::getTimeUs() >= _cursorBlinkTime) {
					setCursorBlinkStateAndTime(!_cursorBlinkState);
					invalidateRender();
				}
			}
		}
	}

	void TextField::onRender(Renderer* renderer, const Rectangle& bounds) {
		const auto focused = isFocused();

		// Background
		const auto backgroundColor = Color::select(focused, _defaultBackgroundColor, _focusedBackgroundColor);

		if (backgroundColor) {
			renderer->fillRectangle(
				bounds,
				getCornerRadius(),
				backgroundColor
			);
		}

		// Border
		const auto borderColor = Color::select(focused, _defaultBorderColor, _focusedBorderColor);

		if (borderColor) {
			renderer->strokeRectangle(
				bounds,
				getCornerRadius(),
				borderColor
			);
		}

		// Text
		const auto font = getFont();

		if (!font)
			return;

		const auto fontHeight = font->getLineHeight(getFontScale());
		const auto& text = getText();

		// Placeholder
		if (text.empty() && !isFocused()) {
			if (_placeholderColor) {
				const auto& placeholder = getPlaceholder();

				if (!placeholder.empty()) {
					renderer->putText(
						Point(
							bounds.getX() + _textMargin,
							bounds.getYCenter() - fontHeight / 2
						),
						font,
						getFontScale(),
						_placeholderColor,
						placeholder
					);
				}
			}
		}
		// Text & cursor
		else {
			const auto textLength = UTF8::getLength(text);

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

				const auto oldClip = renderer->pushClip(Rectangle(
					bounds.getX() + _textMargin,
					bounds.getY(),
					bounds.getWidth() - _textMargin * 2,
					bounds.getHeight()
				));

				size_t charIndex = 0;
				size_t codepointIndex = 0;
				uint32_t codepoint;

				while (charIndex < text.length()) {
					UTF8::nextCodepoint(text, charIndex, codepoint);

					if (_mask) {
						renderer->putText(
							textPosition,
							font,
							getFontScale(),
							textColor,
							_mask
						);
					}
					else {
						renderer->putText(
							textPosition,
							font,
							getFontScale(),
							textColor,
							codepoint
						);
					}

					textPosition.setX(textPosition.getX() + font->getWidth(getFontScale(), codepoint));

					codepointIndex++;

					if (codepointIndex == _cursorPosition)
						blinkX = textPosition.getX();
				}

				renderer->setClip(oldClip);
			}

			// Cursor
			if (_cursorBlinkState && _cursorColor) {
				// End of text
				if (_cursorPosition == textLength)
					blinkX = textPosition.getX();

				renderer->fillRectangle(
					Rectangle(
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
		if (event->is<PointerDownEvent>()) {
			setFocused(true);
			setCaptured(true);

			_lastTouchX = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getX();

			applyContinuousScroll();

			event->setHandled(true);
		}
		else if (event->is<PointerDragEvent>()) {
			_lastTouchX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();

			applyContinuousScroll();

			event->setHandled(true);
		}
		else if (event->is<PointerUpEvent>()) {
			setCaptured(false);

			event->setHandled(true);
		}
		else if (event->is<KeyUpEvent>()) {
			if (!isFocused())
				return;
			
			const auto keyUpEvent = reinterpret_cast<KeyUpEvent*>(event);

//			ESP_LOGI("textField", "keyUpEvent: %d", (int32_t) keyUpEvent->getKey());

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
						insert(text.value());
					}

					break;
				}
			}
			
			onInput(keyUpEvent->getKey(), keyUpEvent->getText());

			if (_onInput)
				_onInput(keyUpEvent->getKey(), keyUpEvent->getText());

			keyUpEvent->setHandled(true);
		}
	}

	void TextField::applyContinuousScroll() {
		const auto font = getFont();

		if (!font)
			return;

		const auto text = getText();
		const auto textLength = UTF8::getLength(text);
		const auto& bounds = getRenderingBounds();
		const int32_t boundsXWithoutMargin = bounds.getX() + _textMargin;
		const uint16_t boundsWidthWithoutMargin = bounds.getWidth() - _textMargin * 2;
		const int32_t boundsX2WithoutMargin = boundsXWithoutMargin + boundsWidthWithoutMargin - 1;

		size_t cursorPosition;
		int32_t cursorX;

		const auto& computeCursorPositionFor = [this, &boundsXWithoutMargin, font, &cursorPosition, &cursorX, text](const int32_t targetX) {
			cursorX = boundsXWithoutMargin - _scrollPosition;
			cursorPosition = 0;

			size_t charIndex = 0;
			uint32_t codepoint;

			while (charIndex < text.length()) {
				UTF8::nextCodepoint(text, charIndex, codepoint);

				if (cursorX < targetX) {
					cursorPosition++;

					cursorX +=
						_mask
						? font->getWidth(_mask)
						: font->getWidth(codepoint);
				}
				else {
					break;
				}
			}

			// Converting to [0 px; Cursor px]
			cursorX -= boundsXWithoutMargin - _scrollPosition;
		};

		if (_lastTouchX < boundsXWithoutMargin) {
			computeCursorPositionFor(boundsXWithoutMargin);

			if (cursorPosition > 0) {
				cursorPosition--;

				if (_scrollPosition > 0) {
					const auto previousCharWidth = font->getWidth(getFontScale(), _mask ? _mask : text[cursorPosition]);

					_scrollPosition = cursorX > previousCharWidth ? cursorX - previousCharWidth : 0;
				}
			}

			setCursorPosition(cursorPosition);
		}
		else if (_lastTouchX > boundsX2WithoutMargin) {
			computeCursorPositionFor(boundsX2WithoutMargin);

			if (cursorPosition < textLength) {
				const int32_t pizda =
					cursorX
					+ font->getWidth(getFontScale(), _mask ? _mask : text[cursorPosition])
					- boundsWidthWithoutMargin;

				_scrollPosition = pizda > 0 ? pizda : 0;

				cursorPosition++;
			}

			setCursorPosition(cursorPosition);
		}
		else {
			computeCursorPositionFor(_lastTouchX);
			setCursorPosition(cursorPosition);
		}

		_continuousScrollTime = system::getTimeUs() + _continuousScrollInterval;
	}

	void TextField::insert(const std::string_view value) {
		const auto text = getText();
		const auto textLength = UTF8::getLength(text);

		// ABCDE
		// -----

		// Start
		if (_cursorPosition == 0) {
			if (text.empty()) {
				setText(value);
			}
			else {
				setText(std::string(value) + std::string(text));
			}
		}
		// End
		else if (_cursorPosition == textLength) {
			setText(std::string(text) + std::string(value));
		}
		// Middle
		else {
			setText(std::string(UTF8::substring(text, 0, _cursorPosition)) + std::string(value) + std::string(UTF8::substring(text, _cursorPosition)));
		}

		setCursorPosition(_cursorPosition + UTF8::getLength(value));
	}

	void TextField::backspace() {
		const auto text = getText();
		const auto textLength = UTF8::getLength(text);

		// Start
		if (_cursorPosition == 0)
			return;

		// End
		if (_cursorPosition == textLength) {
			if (!text.empty()) {
				setText(UTF8::substring(text, 0, textLength - 1));
			}
		}
		// 1
		else if (_cursorPosition == 1) {
			if (textLength > 1) {
				setText(UTF8::substring(text, 1));
			}
			else if (!text.empty()) {
				setText(std::string());
			}
		}
		// Middle
		else {
			setText(std::string(UTF8::substring(text, 0, _cursorPosition - 1)) + std::string(UTF8::substring(text, _cursorPosition)));
		}

		setCursorPosition(_cursorPosition - 1);
	}

	void TextField::setOnInput(const std::function<void(Key, std::optional<std::string_view>)>& callback) {
		_onInput = callback;
	}

	size_t TextField::getCursorPosition() const {
		return _cursorPosition;
	}

	void TextField::setCursorPosition(const size_t value) {
		_cursorPosition = value;

		const auto textLength = UTF8::getLength(getText());

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
		setCursorPosition(UTF8::getLength(getText()));
	}

	void TextField::onFocusChanged() {
		Control::onFocusChanged();

		setCursorBlinkStateAndTime(isFocused());

		if (isFocused()) {
			KeyboardController::show(_keyboardLayoutOptions);

			scrollIntoView();
		}

		invalidateRender();
	}

	void TextField::setCursorBlinkStateAndTime(const bool value) {
		_cursorBlinkState = value;
		_cursorBlinkTime = system::getTimeUs() + _cursorBlinkInterval;
	}

	const uint16_t& TextField::getTextMargin() const {
		return _textMargin;
	}

	void TextField::setTextMargin(const uint16_t& textMargin) {
		_textMargin = textMargin;

		invalidateRender();
	}

	void TextField::onInput(Key key, std::optional<std::string_view> text) {

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

	uint8_t TextField::getKeyboardLayoutOptions() const {
		return _keyboardLayoutOptions;
	}

	void TextField::setKeyboardLayoutOptions(const uint8_t keyboardLayoutOptions) {
		_keyboardLayoutOptions = keyboardLayoutOptions;
	}

	char TextField::getMask() const {
		return _mask;
	}

	void TextField::setMask(const char mask) {
		_mask = mask;
	}

	std::string_view TextField::getPlaceholder() const {
		return _placeholder;
	}

	void TextField::setPlaceholder(const std::string_view value) {
		_placeholder = value;

		invalidate();
	}
}
