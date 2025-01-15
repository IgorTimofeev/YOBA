#pragma once

#include "element.h"
#include "ui/traits/textElement.h"
#include "ui/traits/primaryColorElement.h"
#include "ui/traits/fontElement.h"
#include "rendering/renderer.h"
#include "size.h"

namespace yoba::ui {
	class Text : public TextElement, public FontElement, public PrimaryColorElement {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setPrimaryColor(foreground);
			}

			explicit Text(const Color* foreground) {
				setPrimaryColor(foreground);
			}

			Size onMeasure(Renderer* renderer, const Size& availableSize) override {
				const auto font = getFontOrDefault();

				return
					font
					? Size(
						font->getWidth(getText()),
						font->getHeight()
					)
					: Size();
			}

			void onRender(Renderer* renderer) override {
				const auto font = getFontOrDefault();

				if (!font)
					return;

				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					primaryColor = renderer->getPrimaryColor();

				renderer->renderText(
					getBounds().getTopLeft(),
					font,
					primaryColor,
					getText()
				);
			}
	};
}
