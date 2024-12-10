#pragma once

#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "element.h"
#include "primaryColorElement.h"

namespace yoba {
	class Circle : public virtual Element, public PrimaryColorElement {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getPrimaryColor())
					return;

				const auto& bounds = getBounds();

				screenBuffer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					getPrimaryColor()
				);
			}
	};
}