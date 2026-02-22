#include <YOBA/UI/textView.h>

namespace YOBA {
	TextView::TextView(const Font& font, const Color& foreground) {
		setFont(font);
		setTextColor(foreground);
	}

	TextView::TextView(const std::wstring_view text) {
		setText(text);
	}

	TextView::TextView(const Color& foreground) {
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
		if (!hasFont())
			return {};

		const auto& font = getFont();

		if (_wrappingEnabled) {
			auto result = Size();

			_wrappedLines.clear();

			font.wrap(
				getText(),
				getFontScale(),
				availableSize.getWidth(),
				[this, &result, font](const std::wstring_view line, const uint16_t width) {
					if (width > result.getWidth())
						result.setWidth(width);

					result.setHeight(result.getHeight() + font.getHeight(getFontScale()));

					_wrappedLines.push_back(line);
				}
			);

			return result;
		}

		return {
			font.getWidth(getText(), getFontScale()),
			font.getHeight(getFontScale())
		};
	}

	void TextView::onRender(Renderer& renderer, const Bounds& bounds) {
		if (!hasFont() || !hasTextColor())
			return;

		const auto& font = getFont();
		const auto& color = getTextColor();

		if (_wrappingEnabled) {
			auto position = getBounds().getPosition();

			for (const auto& line : _wrappedLines) {
				renderer.renderText(
					position,
					font,
					color,
					line,
					getFontScale()
				);

				position.setY(position.getY() + font.getHeight(getFontScale()));
			}
		}
		else {
			renderer.renderText(
				getBounds().getPosition(),
				font,
				color,
				getText(),
				getFontScale()
			);
		}
	}
}
