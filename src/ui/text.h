#pragma once

#include "element.h"
#include "textElement.h"
#include "primaryColorElement.h"
#include "fontElement.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "../size.h"

namespace yoba {
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

			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				const auto font = getFontOrDefault();

				return
					font
					? font->getSize(getText())
					: Size();
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				const auto font = getFontOrDefault();

				if (!font)
					return;

				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					primaryColor = screenBuffer->getPrimaryColor();

				screenBuffer->renderText(
					getBounds().getTopLeft(),
					font,
					primaryColor,
					getText()
				);
			}
	};
}
