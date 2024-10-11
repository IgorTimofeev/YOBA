#pragma once

#include "../../event.h"
#include "../element.h"
#include "../traits/fontAware.h"
#include "../traits/foregroundColorAware.h"

namespace yoba {
	class TouchCanvas : public virtual Element, public ForegroundColorAware, public FontAware {
		public:

		protected:
			void onEvent(InputEvent &event) override {
				if (typeid(event) == typeid(TouchDownEvent)) {
					_points.push_back(((TouchDownEvent&) event).getPosition());
				}
				else if (typeid(event) == typeid(PinchDownEvent)) {
					_points.clear();
				}
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getForegroundColor())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				wchar_t text[16];

				for (const auto& point : _points) {
					screenBuffer->renderFilledCircle(point, 5, getForegroundColor());

					swprintf(text, 16, L"%d x %x", point.getX(), point.getY());
					screenBuffer->renderText(Point(point.getX(), point.getY() + 10), font, getForegroundColor(), text);
				}
			}

		private:
			std::vector<Point> _points;
	};
}
