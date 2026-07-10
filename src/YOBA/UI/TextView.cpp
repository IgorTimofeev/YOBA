#include <YOBA/UI/TextView.hpp>
#include <YOBA/Core/Text.hpp>

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

	Alignment TextView::getTextAlignment() const {
		return _textAlignment;
	}

	void TextView::setTextAlignment(const Alignment value) {
		_textAlignment = value;
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

		switch (_textAlignment) {
			case Alignment::start: {
				if (_wrappingEnabled) {
					const auto x = bounds.getX();
					auto y = bounds.getY();

					for (const auto line : _wrappedLines) {
						renderer->putText(
							{
								x,
								y
							},
							font,
							color,
							line,
							getFontScale()
						);

						y += font->getLineHeight(getFontScale());
					}

				}
				else {
					renderer->putText(
						bounds.getPosition(),
						font,
						color,
						getText(),
						getFontScale()
					);
				}

				break;
			}
			case Alignment::end: {
				const auto x2 = bounds.getX2();

				if (_wrappingEnabled) {
					auto y = bounds.getX();

					for (const auto line : _wrappedLines) {
						renderer->putText(
							{
								x2 - font->getWidth(line, getFontScale()) + 1,
								y
							},
							font,
							color,
							line,
							getFontScale()
						);

						y += font->getLineHeight(getFontScale());
					}
				}
				else {
					renderer->putText(
						{
							bounds.getX2() - font->getWidth(getText(), getFontScale()) + 1,
							bounds.getY()
						},
						font,
						color,
						getText(),
						getFontScale()
					);
				}

				break;
			}
			case Alignment::center:
			case Alignment::stretch: {
				const auto xCenter = bounds.getXCenter();

				if (_wrappingEnabled) {
					auto y = bounds.getY();

					for (const auto line : _wrappedLines) {
						renderer->putText(
							{
								xCenter - font->getWidth(line, getFontScale()) / 2,
								y
							},
							font,
							color,
							line,
							getFontScale()
						);

						y += font->getLineHeight(getFontScale());
					}
				}
				else {
					renderer->putText(
						{
							xCenter - font->getWidth(getText(), getFontScale()) / 2,
							bounds.getY()
						},
						font,
						color,
						getText(),
						getFontScale()
					);
				}

				break;
			}
		}
	}
}
