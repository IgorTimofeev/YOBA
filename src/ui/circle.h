#pragma once

#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "shape.h"

namespace yoba {
	class Circle : public Shape {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					primaryColor = screenBuffer->getPrimaryColor();

				const auto& bounds = getBounds();

				screenBuffer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					primaryColor
				);
			}
	};
}