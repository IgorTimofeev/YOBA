#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/textElement.h>
#include <YOBA/UI/traits/textColorElement.h>
#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/traits/fontScaleElement.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/main/size.h>

namespace YOBA {
	class Text : public Control, public TextElement, public FontElement, public FontScaleElement, public TextColorElement {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setTextColor(foreground);
			}

			explicit Text(const std::wstring_view text) {
				setText(text);
			}

			explicit Text(const Color* foreground) {
				setTextColor(foreground);
			}

			bool isWrappingEnabled() const {
				return _wrappingEnabled;
			}

			void setWrappingEnabled(const bool value) {
				if (value == _wrappingEnabled)
					return;

				_wrappingEnabled = value;
				_wrappedLines.clear();

				invalidate();
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
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

			void onRender(Renderer* renderer, const Bounds& bounds) override {
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

		private:
			bool _wrappingEnabled = false;
			std::vector<std::wstring_view> _wrappedLines {};
	};
}
