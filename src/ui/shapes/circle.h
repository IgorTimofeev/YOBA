#pragma once

#include "color.h"
#include "rendering/renderer.h"
#include "shape.h"

namespace yoba::ui {
	class Circle : public Shape {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer) override {
				auto color = getFillColor();

				if (!color)
					return;

				const auto& bounds = getBounds();

				renderer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					color
				);
			}
	};
}