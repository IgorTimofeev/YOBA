#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

namespace YOBA {
	class CircularShape : public Shape {
		public:
			CircularShape() = default;

			CircularShape(const Color* color) {
				setFillColor(color);
			}

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				const auto color = getFillColor();

				if (!color)
					return;

				renderer->fillCircle(
					bounds.getCenter(),
					std::min(bounds.getWidth(), bounds.getHeight()) / 2,
					color
				);
			}
	};
}