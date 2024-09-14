#pragma once

#include "element.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "size.h"
#include "text_aware.h"
#include "foreground_aware.h"
#include "font_aware.h"

namespace yoba {
	class Text : public TextAware, public FontAware, public ForegroundAware {
		public:
			Text() = default;

			Text(const Font* font, const Color* foreground, const String& value) {
				setFont(font);
				setForeground(foreground);
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
