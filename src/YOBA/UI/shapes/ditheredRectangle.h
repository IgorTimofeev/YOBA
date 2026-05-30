#pragma once

#include "shape.h"
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>

namespace YOBA {
	class DitheredRectangleShape : public Shape {
		public:
			DitheredRectangleShape() = default;

			explicit DitheredRectangleShape(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto color = getFillColor();

				if (color)
					renderer->renderDitheredRectangle(bounds, color);
			}
	};
}
