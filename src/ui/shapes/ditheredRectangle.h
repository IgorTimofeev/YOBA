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
					renderer->renderDitheredRectangle(getBounds(), color, _dotSize, _transparencySize);
			}

			uint8_t getDotSize() const {
				return _dotSize;
			}

			void setDotSize(uint8_t dotSize) {
				_dotSize = dotSize;
			}

			uint8_t getTransparencySize() const {
				return _transparencySize;
			}

			void setTransparencySize(uint8_t transparencySize) {
				_transparencySize = transparencySize;
			}

		protected:
			uint8_t _dotSize = 1;
			uint8_t _transparencySize = 1;
	};
}
