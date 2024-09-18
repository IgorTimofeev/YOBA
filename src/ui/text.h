#pragma once

#include "element.h"
#include "traits/textAware.h"
#include "traits/foregroundAware.h"
#include "traits/fontAware.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../size.h"

namespace yoba {
	class Text : public TextAware, public FontAware, public ForegroundAware {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setForeground(foreground);
			}

			explicit Text(const Color* foreground) {
				setForeground(foreground);
			}

			Text(const Color* foreground, const String& text) : Text(foreground) {
				setText(text);
			}

			Text(const Font* font, const Color* foreground, const String& text) : Text(font, foreground) {
				setText(text);
			}

			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				const auto font = getFontOrDefault();

				return
					font
					? font->getSize(getText())
					: Size();
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getForeground())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				screenBuffer->renderString(
					getBounds().getPosition(),
					font,
					getForeground(),
					getText()
				);
			}
	};
}
