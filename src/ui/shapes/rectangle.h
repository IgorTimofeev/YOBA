#pragma once

#include "shape.h"
#include "color.h"
#include "rendering/renderer.h"
#include "ui/traits/cornerRadiusElement.h"

namespace yoba::ui {
	class Rectangle : public Shape, public CornerRadiusElement {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(Renderer* renderer) override {
				auto primaryColor = getPrimaryColor();

				if (primaryColor)
					renderer->renderFilledRectangle(getBounds(), getCornerRadius(), primaryColor);
			}
	};
}
