#pragma once

#include "element.h"
#include "traits/textAware.h"
#include "traits/foregroundColorAware.h"
#include "traits/fontAware.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../size.h"

namespace yoba {
	class Text : public TextAware, public FontAware, public ForegroundColorAware {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setForegroundColor(foreground);
			}

			explicit Text(const Color* foreground) {
				setForegroundColor(foreground);
			}

			Size getDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				const auto font = getFontOrDefault();

				return
					font
					? font->getSize(getText())
					: Size();
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getForegroundColor() || !getText())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				screenBuffer->renderText(
					getBounds().getPosition(),
					font,
					getForegroundColor(),
					getText()
				);
			}
	};
}
