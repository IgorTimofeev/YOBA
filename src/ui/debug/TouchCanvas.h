#pragma once

#include "../../event.h"
#include "../element.h"
#include "../fontAware.h"
#include "../foregroundAware.h"

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
				if (!getForeground() || !getFont())
					return;

				for (const auto& point : _points) {
					screenBuffer->renderFilledRectangle(Bounds(point, Size(10, 10)), getForeground());
					screenBuffer->renderText(Point(point.getX(), point.getY() + 15), getFont(), getForeground(), String(point.getX()) + String(", ") + String(point.getY()));
				}
			}

		private:
			std::vector<Point> _points;
	};
}
