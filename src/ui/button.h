#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "traits/backgroundAware.h"
#include "traits/foregroundAware.h"
#include "traits/textAware.h"
#include "traits/fontAware.h"
#include "traits/cornerRadiusAware.h"
#include "../event.h"

namespace yoba {
	class Button : public TextAware, public FontAware, public BackgroundAware, public ForegroundAware, public CornerRadiusAware {
		public:
			void addOnClick(const std::function<void(Event&)>& value) {
				_onClick.add(value);
			}

		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getForeground() || !getText())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				auto& bounds = getBounds();

				auto textSize = getFont()->getSize(getText());

				screenBuffer->renderFilledRectangle(bounds, getCornerRadius(), getBackground());

				screenBuffer->renderString(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - textSize.getWidth() / 2,
						bounds.getY() + bounds.getHeight() / 2 - textSize.getHeight() / 2
					),
					font,
					getForeground(),
					getText()
				);
			}

			void onEvent(Event &event) override {
				if (event.getType() == EventType::TouchDown)
					_onClick.call(event);
			}

		private:
			Action<Event&> _onClick {};
	};
}
