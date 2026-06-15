#pragma once

#include "shape.h"
#include <YOBA/core/color.h>
#include <YOBA/rendering/renderer.h>
#include <YOBA/UI/traits/borderElement.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>

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
