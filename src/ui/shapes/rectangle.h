#pragma once

#include "shape.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "ui/traits/borderElement.h"
#include "ui/traits/cornerRadiusElement.h"

namespace yoba::ui {
	class Rectangle : public Shape, public BorderElement, public CornerRadiusElement {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto color = getFillColor();

				if (color)
					renderer->renderFilledRectangle(getBounds(), getCornerRadius(), color);

				color = getBorderColor();

				if (color)
					renderer->renderRectangle(getBounds(), getCornerRadius(), color);
			}
	};
}
