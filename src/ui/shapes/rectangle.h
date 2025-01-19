#pragma once

#include "shape.h"
#include "color.h"
#include "rendering/renderers/renderer.h"
#include "ui/traits/cornerRadiusElement.h"

namespace yoba::ui {
	class Rectangle : public Shape, public CornerRadiusElement {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer) override {
				auto color = getFillColor();

				if (color)
					renderer->renderFilledRectangle(getBounds(), getCornerRadius(), color);
			}
	};
}
