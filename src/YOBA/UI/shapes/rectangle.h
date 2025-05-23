#pragma once

#include "shape.h"
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/UI/traits/borderElement.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>

namespace YOBA {
	class Rectangle : public Shape, public BorderElement, public CornerRadiusElement {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto color = getFillColor();

				if (color)
					renderer->renderFilledRectangle(bounds, getCornerRadius(), color);

				color = getBorderColor();

				if (color)
					renderer->renderRectangle(bounds, getCornerRadius(), color);
			}
	};
}
