#pragma once

#include "shape.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "cornerRadiusElement.h"

namespace yoba {
	class Rectangle : public Shape, public CornerRadiusElement {
		public:
			Rectangle() = default;

			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				return {0, 0};
			}

			explicit Rectangle(const Color* color) {
				setPrimaryColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					primaryColor = screenBuffer->getDefaultPrimaryColor();

				if (primaryColor)
					screenBuffer->renderFilledRectangle(getBounds(), getCornerRadius(), primaryColor);
			}
	};
}
