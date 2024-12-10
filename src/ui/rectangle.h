#pragma once

#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "element.h"
#include "primaryColorElement.h"
#include "cornerRadiusElement.h"

namespace yoba {
	class Rectangle : public PrimaryColorElement, public CornerRadiusElement {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (getPrimaryColor())
					screenBuffer->renderFilledRectangle(getBounds(), getCornerRadius(), getPrimaryColor());
			}
	};
}
