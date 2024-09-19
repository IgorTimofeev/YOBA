#pragma once

#include "../../color.h"
#include "../../hardware/screen/buffers/screenBuffer.h"
#include "../element.h"
#include "../traits/backgroundAware.h"
#include "ui/traits/cornerRadiusAware.h"

namespace yoba {
	class Rectangle : public BackgroundAware, public CornerRadiusAware {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setBackground(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (getBackground())
					screenBuffer->renderFilledRectangle(getBounds(), getCornerRadius(), getBackground());
			}
	};
}
