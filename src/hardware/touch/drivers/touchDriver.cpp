#include "touchDriver.h"

namespace yoba {
	void TouchDriver::begin() {
		// Interrupt
		pinMode(_intPin, INPUT_PULLUP);

		attachInterrupt(
			digitalPinToInterrupt(_intPin),
			std::bind(&onInterrupt, this),
			CHANGE
		);

		// I2C
		Wire.begin(_sdaPin, _sclPin);

		// Toggle reset pin
		pinMode(_rstPin, OUTPUT);
		digitalWrite(_rstPin, LOW);
		delay(10);
		digitalWrite(_rstPin, HIGH);
	}

	void TouchDriver::onInterrupt(TouchDriver* driver) {
		driver->_interrupted = true;
	}

	Point TouchDriver::rotatePoint(ScreenBuffer* screenBuffer, Point point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (screenBuffer->getRotation()) {
			// 270
			case ScreenOrientation::Landscape0:
				return {
					point.getY(),
					screenBuffer->getSize().getHeight() - point.getX()
				};

			case ScreenOrientation::Portrait90:
				return point;

			case ScreenOrientation::Landscape270:
				return {
					screenBuffer->getSize().getWidth() - point.getY(),
					point.getX()
				};

				break;
		}

		return point;
	}

	Point TouchDriver::readRotatedPoint1(ScreenBuffer* screenBuffer) {
		return rotatePoint(screenBuffer, readPoint1());
	}

	Point TouchDriver::readRotatedPoint2(ScreenBuffer* screenBuffer) {
		return rotatePoint(screenBuffer, readPoint2());
	}

	void TouchDriver::tick(ScreenBuffer* screenBuffer, const std::function<void(Event&)>& callback) {
		if (!_interrupted)
			return;

		_interrupted = false;

		auto isDown1 = readPoint1Down();
		auto isDown2 = readPoint2Down();

		if (isDown1) {
			if (isDown2) {
				// Pinch drag
				if (_isPinched) {
					auto point1 = readRotatedPoint1(screenBuffer);
					auto point2 = readRotatedPoint2(screenBuffer);

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
					_touchPoints[0].setPosition(readRotatedPoint1(screenBuffer));

					_touchPoints[1].setDown(true);
					_touchPoints[1].setPosition(readRotatedPoint2(screenBuffer));

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
					_touchPoints[1].setPosition(readRotatedPoint2(screenBuffer));

					auto event = PinchUpEvent(
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					);

					callback(event);
				}
					// Touch drag
				else if (_isTouched) {
					auto point1 = readRotatedPoint1(screenBuffer);

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
					_touchPoints[0].setPosition(readRotatedPoint1(screenBuffer));

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
				_touchPoints[1].setPosition(readRotatedPoint2(screenBuffer));

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
				_touchPoints[0].setPosition(readRotatedPoint1(screenBuffer));

				auto event = TouchUpEvent(
					_touchPoints[0].getPosition()
				);

				callback(event);
			}
		}
	}
}