#pragma once

#include <YOBA/UI/Shapes/Shape.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

namespace YOBA {
	class ChessPatternRectangularShape : public Shape {
		public:
			ChessPatternRectangularShape() = default;

			ChessPatternRectangularShape(const Color* color) {
				setFillColor(color);
			}

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				const auto color = getFillColor();

				if (color)
					renderer->fillChessPatternRectangle(bounds, color, 1, 1);
			}
	};
}
