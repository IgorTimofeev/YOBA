#pragma once

#include "../../event.h"
#include "../element.h"
#include "ui/fontElement.h"
#include "ui/secondaryColorElement.h"

namespace yoba {
	class TouchCanvas : public virtual Element, public SecondaryColorElement, public FontElement {
		public:

		protected:
			void onEvent(InputEvent &event) override {
				if (event.getTypeID() == TouchDownEvent::typeID) {
					_points.push_back(((TouchDownEvent&) event).getPosition());
				}
				else if (event.getTypeID() == PinchDownEvent::typeID) {
					_points.clear();
				}
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getSecondaryColor())
					return;

				const auto font = getFontOrDefault();

				if (!font)
					return;

				wchar_t text[16];

				for (const auto& point : _points) {
					screenBuffer->renderFilledCircle(point, 5, getSecondaryColor());

					swprintf(text, 16, L"%d x %x", point.getX(), point.getY());
					screenBuffer->renderText(Point(point.getX(), point.getY() + 10), font, getSecondaryColor(), text);
				}
			}

		private:
			std::vector<Point> _points;
	};
}
