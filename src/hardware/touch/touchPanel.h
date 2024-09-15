#pragma once

#include "drivers/touchDriver.h"
#include "../../action.h"

namespace yoba {
	class TouchPanel {
		public:
			explicit TouchPanel(TouchDriver* driver);

			void begin() {
				_driver->begin();
			}

			void addOnTouchDown(const std::function<void(const Point&)>& callback) {
				_onTouchDown.add(callback);
			}

			void addOnTouchDrag(const std::function<void(const Point&)>& callback) {
				_onTouchDrag.add(callback);
			}

			void addOnTouchUp(const std::function<void(const Point&)>& callback) {
				_onTouchUp.add(callback);
			}

			void addOnPinchDown(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchDown.add(callback);
			}

			void addOnPinchDrag(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchDrag.add(callback);
			}

			void addOnPinchUp(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchUp.add(callback);
			}

			void tick();

		protected:
			TouchDriver* _driver;

			// -------------------------------- Touch --------------------------------

			bool _isTouched = false;
			bool _isPinched = false;

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			Action<const Point&> _onTouchDown;
			Action<const Point&> _onTouchDrag;
			Action<const Point&> _onTouchUp;
			Action<const Point&, const Point&> _onPinchDown;
			Action<const Point&, const Point&> _onPinchDrag;
			Action<const Point&, const Point&> _onPinchUp;

			Point rotateTouchPoint(Point point);
			Point readTouchPoint1();
			Point readTouchPoint2();
			void callOnTouchDown();
			void callOnTouchDrag();
			void callOnTouchUp();
			void callOnPinchDown();
			void callOnPinchDrag();
			void callOnPinchUp();
	};
}