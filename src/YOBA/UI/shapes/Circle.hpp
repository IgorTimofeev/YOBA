#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Color.hpp>
#include <YOBA/Rendering/Renderer.hpp>

namespace YOBA {
	class CircleShape : public Shape {
		public:
			CircleShape() = default;

			explicit CircleShape(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
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