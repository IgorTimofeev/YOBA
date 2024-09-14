#pragma once

#include "shape.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class Rectangle : public Shape {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				screenBuffer->renderFilledRectangle(getBounds(), getFillColor());
			}

		private:

	};
}
