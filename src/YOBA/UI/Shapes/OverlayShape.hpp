#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

namespace YOBA {
	class OverlayShape : public Shape {
		public:
			OverlayShape() = default;

			OverlayShape(const Color* color) {
				setFillColor(color);
			}

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				const auto color = getFillColor();

				if (!color)
					return;

				// Full rich bitch VIP alpha-blending
				if (color->getModel() == ColorModel::ARGB) {
					renderer->fillRectangle(bounds, color);
				}
				// Simulating alpha-blending via chess pattern
				else {
					renderer->fillChessPatternRectangle(bounds, color, 1, 1);
				}
			}
	};
}
