#pragma once

#include "shape.h"
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>

namespace YOBA {
	class DitheredRectangle : public Shape {
		public:
			DitheredRectangle() = default;

			explicit DitheredRectangle(const Color& color) {
				setFillColor(color);
			}

			void onRender(Renderer& renderer, const Bounds& bounds) override {
				if (hasFillColor())
					renderer.renderDitheredRectangle(getBounds(), getFillColor());
			}
	};
}
