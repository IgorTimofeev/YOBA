#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Color.hpp>
#include <YOBA/Rendering/Renderer.hpp>
#include <YOBA/UI/Traits/BorderElement.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>

namespace YOBA {
	class RectangleShape : public Shape, public BorderElement, public CornerRadiusElement {
		public:
			RectangleShape() = default;

			explicit RectangleShape(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				auto color = getFillColor();

				if (color)
					renderer->renderFilledRectangle(bounds, getCornerRadius(), color);

				color = getBorderColor();

				if (color)
					renderer->renderRectangle(bounds, getCornerRadius(), color);
			}
	};
}
