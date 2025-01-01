#pragma once

#include "element.h"
#include "textElement.h"
#include "secondaryColorElement.h"
#include "fontElement.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "../size.h"

namespace yoba {
	class Text : public TextElement, public FontElement, public SecondaryColorElement {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setSecondaryColor(foreground);
			}

			explicit Text(const Color* foreground) {
				setSecondaryColor(foreground);
			}

			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				const auto font = getFontOrDefault();

				return
					font
					? font->getSize(getText())
					: Size();
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getSecondaryColor() || !getText())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				screenBuffer->renderText(
					getBounds().getTopLeft(),
					font,
					getSecondaryColor(),
					getText()
				);
			}
	};
}
