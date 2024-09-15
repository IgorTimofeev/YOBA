#pragma once

#include "element.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "size.h"
#include "textAware.h"
#include "foregroundAware.h"
#include "fontAware.h"

namespace yoba {
	class Text : public TextAware, public FontAware, public ForegroundAware {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground) {
				setFont(font);
				setForeground(foreground);
			}

			Text(const Font* font, const Color* foreground, const String& value) : Text(font, foreground) {
				setText(value);
			}

			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				return
					getFont()
					? screenBuffer->getTextSize(getFont(), getText())
					: Size();
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getFont() || !getForeground())
					return;

				screenBuffer->renderText(
					getBounds().getPosition(),
					getFont(),
					getForeground(),
					getText()
				);
			}
	};
}
