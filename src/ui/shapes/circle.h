#pragma once

#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "ui/element.h"
#include "ui/traits/backgroundAware.h"

namespace yoba {
	class Circle : public virtual Element, public BackgroundAware {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setBackground(color);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getBackground())
					return;

				const auto& bounds = getBounds();

				screenBuffer->renderCircle(
					Point(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2),
					min(bounds.getWidth(), bounds.getHeight()) / 2,
					getBackground()
				);
			}
	};
}