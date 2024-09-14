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
				display.renderRectangle(getBounds(), getCornerRadius(), getFillColor());
			}

			// -------------------------------- Getters & setters --------------------------------

			uint8_t getCornerRadius() const {
				return _cornerRadius;
			}

			void setCornerRadius(uint8_t value) {
				_cornerRadius = value;
			}

		private:
			uint8_t _cornerRadius = 0;
	};
}
