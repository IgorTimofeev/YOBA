#pragma once

#include "shape.h"
#include <YOBA/core/color.h>
#include <YOBA/rendering/renderer.h>

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
