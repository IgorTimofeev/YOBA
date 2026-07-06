#include <YOBA/UI/textView.h>
#include <YOBA/core/text.h>

namespace YOBA {
	TextView::TextView(const Font* font, const Color* foreground) {
		setFont(font);
		setTextColor(foreground);
	}

	TextView::TextView(const std::string_view text) {
		setText(text);
	}

	TextView::TextView(const Color* foreground) {
		setTextColor(foreground);
	}

	bool TextView::isWrappingEnabled() const {
		return _wrappingEnabled;
	}

	void TextView::setWrappingEnabled(const bool value) {
		if (value == _wrappingEnabled)
			return;

		_wrappingEnabled = value;
		_wrappedLines.clear();

		invalidate();
	}

	Size TextView::onMeasure(const Size& availableSize) {
		const auto font = getFont();

		if (!font)
			return {};

		if (_wrappingEnabled) {
			auto result = Size();

			_wrappedLines.clear();

			Text::wrap(
				font,
				getText(),
				getFontScale(),
				availableSize.getWidth(),
				[this, &result, font](const std::string_view line, const uint16_t width) {
					if (width > result.getWidth())
						result.setWidth(width);

					result.setHeight(result.getHeight() + font->getLineHeight(getFontScale()));

					_wrappedLines.push_back(line);
				}
			);

			return result;
		}

		return {
			font->getWidth(getText(), getFontScale()),
			font->getLineHeight(getFontScale())
		};
	}

	void TextView::onRender(Renderer* renderer, const Rectangle& bounds) {
		const auto font = getFont();

		if (!font)
			return;

		const auto color = getTextColor();

		if (!color)
			return;

		if (_wrappingEnabled) {
			auto position = bounds.getPosition();

			for (const auto& line : _wrappedLines) {
				renderer->renderText(
					position,
					font,
					color,
					line,
					getFontScale()
				);

				position.setY(position.getY() + font->getLineHeight(getFontScale()));
			}
		}
		else {
			renderer->renderText(
				bounds.getPosition(),
				font,
				color,
				getText(),
				getFontScale()
			);
		}
	}
}
