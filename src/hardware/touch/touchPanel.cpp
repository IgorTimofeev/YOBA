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

	void TouchPanel::tick(const std::function<void(Event&)>& callback) {
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

						auto event = PinchDragEvent(
							_touchPoints[0].getPosition(),
							_touchPoints[1].getPosition()
						);

						callback(event);
					}
				}
				// Pinch down
				else {
					_isPinched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					_touchPoints[1].setDown(true);
					_touchPoints[1].setPosition(readTouchPoint2());

					auto event = PinchDownEvent(
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					);

					callback(event);
				}
			}
			else {
				// Pinch up
				if (_isPinched) {
					_isPinched = false;

					_touchPoints[1].setDown(false);
					_touchPoints[1].setPosition(readTouchPoint2());

					auto event = PinchUpEvent(
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					);

					callback(event);
				}
				// Touch drag
				else if (_isTouched) {
					auto point1 = readTouchPoint1();

					if (point1 != _touchPoints[0].getPosition()) {
						_touchPoints[0].setPosition(point1);

						auto event = TouchDragEvent(
							_touchPoints[0].getPosition()
						);

						callback(event);
					}
				}
				// Touch down
				else {
					_isTouched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					auto event = TouchDownEvent(
						_touchPoints[0].getPosition()
					);

					callback(event);
				}
			}
		}
		else {
			// Pinch up
			if (_isPinched) {
				_isPinched = false;

				_touchPoints[1].setDown(false);
				_touchPoints[1].setPosition(readTouchPoint2());

				auto event = PinchUpEvent(
					_touchPoints[0].getPosition(),
					_touchPoints[1].getPosition()
				);

				callback(event);
			}

			// Touch up
			if (_isTouched) {
				_isTouched = false;

				_touchPoints[0].setDown(false);
				_touchPoints[0].setPosition(readTouchPoint1());

				auto event = TouchUpEvent(
					_touchPoints[0].getPosition()
				);

				callback(event);
			}
		}
	}
}
