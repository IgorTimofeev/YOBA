#pragma once

#include "element.h"
#include "traits/textElement.h"
#include "traits/textColorElement.h"
#include "traits/fontElement.h"
#include "traits/fontScaleElement.h"
#include "rendering/renderers/renderer.h"
#include "size.h"

namespace yoba::ui {
	class Text : public TextElement, public FontElement, public FontScaleElement, public TextColorElement {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setTextColor(foreground);
			}

			explicit Text(const Color* foreground) {
				setTextColor(foreground);
			}

			bool isWrappingEnabled() const {
				return _wrappingEnabled;
			}

			void setWrappingEnabled(bool value) {
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
					return Size();

				if (_wrappingEnabled) {
					auto result = Size();
					_wrappedLines.clear();

					font->wrap(
						getText(),
						getFontScale(),
						availableSize.getWidth(),
						[this, &result, font](const std::wstring_view& line, uint16_t width) {
							if (width > result.getWidth())
								result.setWidth(width);

							result.setHeight(result.getHeight() + font->getHeight(getFontScale()));

							_wrappedLines.push_back(line);
						}
					);

					return result;
				}
				else {
					return Size(
						font->getWidth(getText(), getFontScale()),
						font->getHeight(getFontScale())
					);
				}
			}

			void onRender(Renderer* renderer) override {
				const auto font = getFont();

				if (!font)
					return;

				auto color = getTextColor();

				if (!color)
					return;

				if (_wrappingEnabled) {
					auto position = getBounds().getPosition();

					for (auto& line : _wrappedLines) {
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
