#include <YOBA/UI/text.h>

namespace YOBA {
	Text::Text(const Font* font, const Color* foreground) {
		setFont(font);
		setTextColor(foreground);
	}

	Text::Text(const std::wstring_view text) {
		setText(text);
	}

	Text::Text(const Color* foreground) {
		setTextColor(foreground);
	}

	bool Text::isWrappingEnabled() const {
		return _wrappingEnabled;
	}

	void Text::setWrappingEnabled(const bool value) {
		if (value == _wrappingEnabled)
			return;

		_wrappingEnabled = value;
		_wrappedLines.clear();

		invalidate();
	}

	Size Text::onMeasure(const Size& availableSize) {
		const auto font = getFont();

		if (!font)
			return {};

		if (_wrappingEnabled) {
			auto result = Size();

			_wrappedLines.clear();

			font->wrap(
				getText(),
				getFontScale(),
				availableSize.getWidth(),
				[this, &result, font](const std::wstring_view line, const uint16_t width) {
					if (width > result.getWidth())
						result.setWidth(width);

					result.setHeight(result.getHeight() + font->getHeight(getFontScale()));

					_wrappedLines.push_back(line);
				}
			);

			return result;
		}

		return {
			font->getWidth(getText(), getFontScale()),
			font->getHeight(getFontScale())
		};
	}

	void Text::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto font = getFont();

		if (!font)
			return;

		const auto color = getTextColor();

		if (!color)
			return;

		if (_wrappingEnabled) {
			auto position = getBounds().getPosition();

			for (const auto& line : _wrappedLines) {
				renderer->renderString(
					position,
					font,
					color,
					line,
					getFontScale()
				);

				position.setY(position.getY() + font->getHeight(getFontScale()));
			}
		}
		else {
			renderer->renderString(
				getBounds().getPosition(),
				font,
				color,
				getText(),
				getFontScale()
			);
		}
	}
}
