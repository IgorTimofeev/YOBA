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

			Size onMeasure(const Size& availableSize) override {
				const auto font = getFont();

				return
					font
					? Size(
						font->getWidth(getText(), getFontScale()),
						font->getHeight(getFontScale())
					)
					: Size();
			}

			void onRender(Renderer* renderer) override {
				const auto font = getFont();

				if (!font)
					return;

				auto color = getTextColor();

				if (color) {
					renderer->renderString(
						getBounds().getPosition(),
						font,
						color,
						getText(),
						getFontScale()
					);
				}
			}
	};
}
