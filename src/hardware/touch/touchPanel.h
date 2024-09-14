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

			void readTouch();

		protected:
			TouchDriver* _driver;

			// -------------------------------- Touch --------------------------------

			bool _isTouched = false;
			bool _isPinched = false;

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			virtual void onTouchDown(const Point& point);
			virtual void onTouchDrag(const Point& point);
			virtual void onTouchUp(const Point& point);
			virtual void onPinchDown(const Point& point1, const Point& point2);
			virtual void onPinchDrag(const Point& point1, const Point& point2);
			virtual void onPinchUp(const Point& point1, const Point& point2);

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