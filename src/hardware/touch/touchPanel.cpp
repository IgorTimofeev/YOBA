#include "touchPanel.h"

namespace yoba {
	TouchPanel::TouchPanel(TouchDriver* driver) : _driver(driver) {

	}

	Point TouchPanel::rotateTouchPoint(Point point) {
		// TODO add rotation support
//		switch (_tft.getRotation()) {
//			// 270
//			case 1:
//				auto tmp = x;
//				x = y;
//				y = _resolution.getHeight() - tmp;
//
//				break;
//		}

		return point;
	}

	Point TouchPanel::readTouchPoint1() {
		return rotateTouchPoint(_driver->readPoint1());
	}

	Point TouchPanel::readTouchPoint2() {
		return rotateTouchPoint(_driver->readPoint2());
	}

	void TouchPanel::callOnTouchDown() {
		auto& point = _touchPoints[0].getPosition();

		_onTouchDown.call(point);
	}

	void TouchPanel::callOnTouchDrag() {
		auto& point = _touchPoints[0].getPosition();

		_onTouchDrag.call(point);
	}

	void TouchPanel::callOnTouchUp() {
		auto& point = _touchPoints[0].getPosition();

		_onTouchUp.call(point);
	}

	void TouchPanel::callOnPinchDown() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		_onPinchDown.call(point1, point2);
	}

	void TouchPanel::callOnPinchDrag() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		_onPinchDrag.call(point1, point2);
	}

	void TouchPanel::callOnPinchUp() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		_onPinchUp.call(point1, point2);
	}

	void TouchPanel::tick() {
		if (!_driver->getInterruptFlag())
			return;

		_driver->clearInterruptFlag();

		auto isDown1 = _driver->readPoint1Down();
		auto isDown2 = _driver->readPoint2Down();

		if (isDown1) {
			if (isDown2) {
				// Pinch drag
				if (_isPinched) {
					auto point1 = readTouchPoint1();
					auto point2 = readTouchPoint2();

					if (
						point1 != _touchPoints[0].getPosition()
						|| point2 != _touchPoints[1].getPosition()
					) {
						_touchPoints[0].setPosition(point1);
						_touchPoints[1].setPosition(point2);

						callOnPinchDrag();
					}
				}
				// Pinch down
				else {
					_isPinched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					_touchPoints[1].setDown(true);
					_touchPoints[1].setPosition(readTouchPoint2());

					callOnPinchDown();
				}
			}
			else {
				// Pinch up
				if (_isPinched) {
					_isPinched = false;

					_touchPoints[1].setDown(false);
					_touchPoints[1].setPosition(readTouchPoint2());

					callOnPinchUp();
				}
				// Touch drag
				else if (_isTouched) {
					auto point1 = readTouchPoint1();

					if (point1 != _touchPoints[0].getPosition()) {
						_touchPoints[0].setPosition(point1);

						callOnTouchDrag();
					}
				}
				// Touch down
				else {
					_isTouched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					callOnTouchDown();
				}
			}
		}
		else {
			// Pinch up
			if (_isPinched) {
				_isPinched = false;

				_touchPoints[1].setDown(false);
				_touchPoints[1].setPosition(readTouchPoint2());

				callOnPinchUp();
			}

			// Touch up
			if (_isTouched) {
				_isTouched = false;

				_touchPoints[0].setDown(false);
				_touchPoints[0].setPosition(readTouchPoint1());

				callOnTouchUp();
			}
		}
	}
}
