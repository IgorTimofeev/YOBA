#pragma once

#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "ui/element.h"
#include "ui/traits/backgroundAware.h"

namespace yoba {
	class Rectangle : public virtual Element, public BackgroundAware {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setBackground(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (getBackground())
					screenBuffer->renderFilledRectangle(getBounds(), getBackground());
			}
	};
}
