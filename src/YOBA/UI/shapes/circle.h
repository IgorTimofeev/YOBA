#pragma once

#include "YOBA/main/color.h"
#include "YOBA/main/rendering/renderer.h"
#include "shape.h"

namespace YOBA {
	class Circle : public Shape {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto color = getFillColor();

				if (!color)
					return;

				renderer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					std::min(bounds.getWidth(), bounds.getHeight()) / 2,
					color
				);
			}
	};
}