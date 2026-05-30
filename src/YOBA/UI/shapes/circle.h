#pragma once

#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include "shape.h"

namespace YOBA {
	class CircleShape : public Shape {
		public:
			CircleShape() = default;

			explicit CircleShape(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto color = getFillColor();

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