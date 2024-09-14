#pragma once

#include "element.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "background_aware.h"
#include "foreground_aware.h"
#include "text_aware.h"
#include "font_aware.h"
#include "../event.h"

namespace yoba {
	class Button : public TextAware, public FontAware, public BackgroundAware, public ForegroundAware {
		public:
			void addOnClick(const std::function<void(Event&)>& value) {
				_onClick.add(value);
			}

		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				auto& bounds = getBounds();

				auto& text = getText();
				auto textSize = screenBuffer->getTextSize(getFont(), text);

				screenBuffer->renderFilledRectangle(bounds, getBackground());

				screenBuffer->renderText(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - textSize.getWidth() / 2,
						bounds.getY() + bounds.getHeight() / 2 - textSize.getHeight() / 2
					),
					getFont(),
					getForeground(),
					text
				);
			}

			void onEvent(Event &event) override {
				if (event.getType() != EventType::touchDown && event.getType() != EventType::touchDrag)
					return;

				_onClick.call(event);
			}

		private:
			Action<Event&> _onClick {};
	};
}
