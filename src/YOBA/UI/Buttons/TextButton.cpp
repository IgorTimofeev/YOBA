#include <YOBA/UI/Buttons/TextButton.hpp>

namespace YOBA {
	TextButton::TextButton(const std::string_view text) {
		setText(text);
	}

	const Margin& TextButton::getTextMargin() const {
		return _textMargin;
	}

	void TextButton::setTextMargin(const Margin& value) {
		_textMargin = value;

		invalidate();
	}

	void TextButton::onRender(Renderer* renderer, const Rectangle& bounds) {
		Button::onRender(renderer, bounds);

		// Text
		const auto font = getFont();

		if (!font)
			return;

		const auto color = Color::select(isActive(), _defaultTextColor, _activeTextColor);

		if (!color)
			return;

		renderer->putText(
			Point(
				bounds.getXCenter() - font->getWidth(getFontScale(), getText()) / 2 + _textMargin.getLeft() - _textMargin.getRight(),
				bounds.getYCenter() - font->getLineHeight(getFontScale()) / 2 + _textMargin.getTop() - _textMargin.getBottom()
			),
			font,
			getFontScale(),
			color,
			getText()
		);
	}

	const Color* TextButton::getDefaultTextColor() const {
		return _defaultTextColor;
	}

	void TextButton::setDefaultTextColor(const Color* value) {
		_defaultTextColor = value;
	}

	const Color* TextButton::getActiveTextColor() const {
		return _activeTextColor;
	}

	void TextButton::setActiveTextColor(const Color* value) {
		_activeTextColor = value;
	}
}
