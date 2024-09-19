#pragma once

#include "../../event.h"
#include "../element.h"
#include "../traits/fontAware.h"
#include "../traits/foregroundAware.h"

namespace yoba {
	class TouchCanvas : public virtual Element, public ForegroundAware, public FontAware {
		public:

		protected:
			void onEvent(Event &event) override {
				switch (event.getType()) {
					case EventType::TouchDown:
						_points.push_back(((TouchDownEvent&) event).getPosition());
						break;

					case EventType::PinchDown:
						_points.clear();
						break;

					default:
						break;
				}
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getForeground())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				wchar_t buffer[16];

				for (const auto& point : _points) {
					screenBuffer->renderFilledCircle(point, 5, getForeground());

					swprintf(buffer, 16, L"%d x %x", point.getX(), point.getY());
					screenBuffer->renderText(Point(point.getX(), point.getY() + 10), font, getForeground(), buffer);
				}
			}

		private:
			std::vector<Point> _points;
	};
}
