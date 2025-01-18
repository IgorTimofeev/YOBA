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

			Size onMeasure(const Size& availableSize) override {
				const auto font = getFont();

				return
					font
					? Size(
						font->getWidth(getText()),
						font->getHeight()
					)
					: Size();
			}

			void onRender(Renderer* renderer) override {
				const auto font = getFont();

				if (!font)
					return;

				auto primaryColor = getPrimaryColor();

				if (primaryColor) {
					renderer->renderString(
						getBounds().getPosition(),
						font,
						primaryColor,
						getText()
					);
				}
			}
	};
}
