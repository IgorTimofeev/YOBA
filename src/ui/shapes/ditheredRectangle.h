#pragma once

#include "shape.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"

namespace yoba::ui {
	class DitheredRectangle : public Shape {
		public:
			DitheredRectangle() = default;

			explicit DitheredRectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto color = getFillColor();

				if (color)
					renderer->renderDitheredRectangle(getBounds(), color);
			}
	};
}
