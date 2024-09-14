#pragma once

#include "shape.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "cmath"

namespace yoba {
	class Circle : public Shape {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				auto bounds = getBounds();

				display.renderCircle(
					Point(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					getFillColor()
				);
			}

			// -------------------------------- Getters & setters --------------------------------

		private:

	};
}
