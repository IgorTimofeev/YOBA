#pragma once

#include "drivers/touchDriver.h"
#include "../../action.h"
#include "../../event.h"

namespace yoba {
	class TouchPanel {
		public:
			explicit TouchPanel(TouchDriver* driver);

			void begin() {
				_driver->begin();
			}

			void tick(const std::function<void(Event&)>& callback);

		protected:
			TouchDriver* _driver;

			// -------------------------------- Touch --------------------------------

			bool _isTouched = false;
			bool _isPinched = false;

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			std::vector<Event*> _eventBuffer;

			Point rotateTouchPoint(Point point);
			Point readTouchPoint1();
			Point readTouchPoint2();
	};
}