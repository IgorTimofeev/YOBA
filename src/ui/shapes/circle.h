#pragma once

#include "color.h"
#include "rendering/renderer.h"
#include "shape.h"

namespace yoba::ui {
	class Circle : public Shape {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(Renderer* renderer) override {
				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					return;

				const auto& bounds = getBounds();

				renderer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					primaryColor
				);
			}
	};
}