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

			explicit Rectangle(const Color& color) {
				setFillColor(color);
			}

			void onRender(Renderer& renderer, const Bounds& bounds) override {
				if (hasFillColor())
					renderer.renderFilledRectangle(bounds, getCornerRadius(), getFillColor());

				if (hasBorderColor())
					renderer.renderRectangle(bounds, getCornerRadius(), getBorderColor());
			}
	};
}
