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

	void TouchPanel::onTouchDown(const Point& point) {

	}

	void TouchPanel::onTouchDrag(const Point& point) {

	}

	void TouchPanel::onTouchUp(const Point& point) {

	}

	void TouchPanel::onPinchDown(const Point& point1, const Point& point2) {

	}

	void TouchPanel::onPinchDrag(const Point& point1, const Point& point2) {

	}

	void TouchPanel::onPinchUp(const Point& point1, const Point& point2) {

	}

	void TouchPanel::callOnTouchDown() {
		auto& point = _touchPoints[0].getPosition();

		onTouchDown(point);
		_onTouchDown.call(point);
	}

	void TouchPanel::callOnTouchDrag() {
		auto& point = _touchPoints[0].getPosition();

		onTouchDrag(point);
		_onTouchDrag.call(point);
	}

	void TouchPanel::callOnTouchUp() {
		auto& point = _touchPoints[0].getPosition();

		onTouchUp(point);
		_onTouchUp.call(point);
	}

	void TouchPanel::callOnPinchDown() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchDown(point1, point2);
		_onPinchDown.call(point1, point2);
	}

	void TouchPanel::callOnPinchDrag() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchDrag(point1, point2);
		_onPinchDrag.call(point1, point2);
	}

	void TouchPanel::callOnPinchUp() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchUp(point1, point2);
		_onPinchUp.call(point1, point2);
	}

	void TouchPanel::readTouch() {
		if (!_driver->getInterruptFlag())
			return;

		_driver->clearInterruptFlag();

		auto isDown1 = _driver->isPoint1Down();
		auto isDown2 = _driver->isPoint2Down();

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
