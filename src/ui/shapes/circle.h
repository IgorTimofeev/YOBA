#pragma once

#include "../../color.h"
#include "../../hardware/screen/buffers/screenBuffer.h"
#include "../element.h"
#include "../traits/backgroundColorAware.h"

namespace yoba {
	class Circle : public virtual Element, public BackgroundColorAware {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setBackgroundColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getBackgroundColor())
					return;

				const auto& bounds = getBounds();

				screenBuffer->renderFilledCircle(
					Point(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					getBackgroundColor()
				);
			}
	};
}