#pragma once

#include "element.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "bounds.h"
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
				display.setFont(getFont());

				return display.measureText(getText());
			}

			void onRender(ScreenBuffer* screenBuffer) override {
//				display.drawRectangle(getBounds(), Color::gold);

				display.setFont(getFont());
				display.renderText(getBounds().getPosition(), getForeground(), getText());
			}
	};
}
