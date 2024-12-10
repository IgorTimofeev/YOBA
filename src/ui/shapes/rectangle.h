#pragma once

#include "../../color.h"
#include "../../hardware/screen/buffers/generic/screenBuffer.h"
#include "../element.h"
#include "../traits/backgroundColorAware.h"
#include "../traits/cornerRadiusAware.h"

namespace yoba {
	class Rectangle : public BackgroundColorAware, public CornerRadiusAware {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setBackgroundColor(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (getBackgroundColor())
					screenBuffer->renderFilledRectangle(getBounds(), getCornerRadius(), getBackgroundColor());
			}
	};
}
