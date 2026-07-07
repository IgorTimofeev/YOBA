#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Color.hpp>
#include <YOBA/Rendering/Renderer.hpp>

namespace YOBA {
	class DitheredRectangleShape : public Shape {
		public:
			DitheredRectangleShape() = default;

			explicit DitheredRectangleShape(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				const auto color = getFillColor();

				if (color)
					renderer->renderDitheredRectangle(bounds, color);
			}
	};
}
