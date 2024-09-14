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

			explicit Text(const String& value) {
				setText(value);
			}

			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				return screenBuffer->getTextSize(getFont(), getText());
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				screenBuffer->renderText(
					getBounds().getPosition(),
					getFont(),
					getForeground(),
					getText()
				);
			}
	};
}
