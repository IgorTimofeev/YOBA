#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Color.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/UI/Traits/BorderElement.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>

namespace YOBA {
	class RectangleShape : public Shape, public BorderElement, public CornerRadiusElement {
		public:
			RectangleShape() = default;

			RectangleShape(const Color* color) {
				setFillColor(color);
			}

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				auto color = getFillColor();

				if (color)
					renderer->fillRectangle(bounds, getCornerRadius(), color);

				color = getBorderColor();

				if (color)
					renderer->strokeRectangle(bounds, getCornerRadius(), color);
			}
	};
}
