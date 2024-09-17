/**************************************************************************/
/*!
  @file     FT6336U.cpp
  Author: Atsushi Sasaki (https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include "FT6336UDriver.h"
#include <Wire.h>

namespace yoba {
	FT6336UDriver::FT6336UDriver(uint8_t rstPin, uint8_t intPin, uint8_t sdaPin, uint8_t sclPin) : _sdaPin(sdaPin), _sclPin(sclPin), _rstPin(rstPin), _intPin(intPin) {

	}

	void FT6336UDriver::begin() {
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

		// Do we need some delay? Hmmm
		//    delay(500);
	}

	void FT6336UDriver::onInterrupt(FT6336UDriver* driver) {
		driver->_interrupted = true;
	}

	uint8_t FT6336UDriver::read_device_mode() {
		return (readByte(FT6336U_ADDR_DEVICE_MODE) & 0x70) >> 4;
	}

	void FT6336UDriver::write_device_mode(DEVICE_MODE_Enum mode) {
		writeByte(FT6336U_ADDR_DEVICE_MODE, (mode & 0x07) << 4);
	}

	uint8_t FT6336UDriver::read_gesture_id() {
		return readByte(FT6336U_ADDR_GESTURE_ID);
	}

	uint8_t FT6336UDriver::read_td_status() {
		return readByte(FT6336U_ADDR_TD_STATUS);
	}

	uint8_t FT6336UDriver::read_touch_number() {
		return readByte(FT6336U_ADDR_TD_STATUS) & 0x0F;
	}

// Touch 1 functions
	uint16_t FT6336UDriver::read_touch1_x() {
		uint8_t buffer[2];
		buffer[0] = readByte(FT6336U_ADDR_TOUCH1_X);
		buffer[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint16_t FT6336UDriver::read_touch1_y() {
		uint8_t buffer[2];
		buffer[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
		buffer[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint8_t FT6336UDriver::read_touch1_event() {
		return readByte(FT6336U_ADDR_TOUCH1_EVENT) >> 6;
	}

	uint8_t FT6336UDriver::read_touch1_id() {
		return readByte(FT6336U_ADDR_TOUCH1_ID) >> 4;
	}

	uint8_t FT6336UDriver::read_touch1_weight() {
		return readByte(FT6336U_ADDR_TOUCH1_WEIGHT);
	}

	uint8_t FT6336UDriver::read_touch1_misc() {
		return readByte(FT6336U_ADDR_TOUCH1_MISC) >> 4;
	}

// Touch 2 functions
	uint16_t FT6336UDriver::read_touch2_x() {
		uint8_t buffer[2];
		buffer[0] = readByte(FT6336U_ADDR_TOUCH2_X);
		buffer[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint16_t FT6336UDriver::read_touch2_y() {
		uint8_t buffer[2];
		buffer[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
		buffer[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint8_t FT6336UDriver::read_touch2_event() {
		return readByte(FT6336U_ADDR_TOUCH2_EVENT) >> 6;
	}

	uint8_t FT6336UDriver::read_touch2_id() {
		return readByte(FT6336U_ADDR_TOUCH2_ID) >> 4;
	}

	uint8_t FT6336UDriver::read_touch2_weight() {
		return readByte(FT6336U_ADDR_TOUCH2_WEIGHT);
	}

	uint8_t FT6336UDriver::read_touch2_misc() {
		return readByte(FT6336U_ADDR_TOUCH2_MISC) >> 4;
	}

// Mode Parameter Register
	uint8_t FT6336UDriver::read_touch_threshold() {
		return readByte(FT6336U_ADDR_THRESHOLD);
	}

	uint8_t FT6336UDriver::read_filter_coefficient() {
		return readByte(FT6336U_ADDR_FILTER_COE);
	}

	uint8_t FT6336UDriver::read_ctrl_mode() {
		return readByte(FT6336U_ADDR_CTRL);
	}

	void FT6336UDriver::write_ctrl_mode(CTRL_MODE_Enum mode) {
		writeByte(FT6336U_ADDR_CTRL, mode);
	}

	uint8_t FT6336UDriver::read_time_period_enter_monitor() {
		return readByte(FT6336U_ADDR_TIME_ENTER_MONITOR);
	}

	uint8_t FT6336UDriver::read_active_rate() {
		return readByte(FT6336U_ADDR_ACTIVE_MODE_RATE);
	}

	uint8_t FT6336UDriver::read_monitor_rate() {
		return readByte(FT6336U_ADDR_MONITOR_MODE_RATE);
	}

// Gesture Parameters
	uint8_t FT6336UDriver::read_radian_value() {
		return readByte(FT6336U_ADDR_RADIAN_VALUE);
	}

	void FT6336UDriver::write_radian_value(uint8_t val) {
		writeByte(FT6336U_ADDR_RADIAN_VALUE, val);
	}

	uint8_t FT6336UDriver::read_offset_left_right() {
		return readByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT);
	}

	void FT6336UDriver::write_offset_left_right(uint8_t val) {
		writeByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT, val);
	}

	uint8_t FT6336UDriver::read_offset_up_down() {
		return readByte(FT6336U_ADDR_OFFSET_UP_DOWN);
	}

	void FT6336UDriver::write_offset_up_down(uint8_t val) {
		writeByte(FT6336U_ADDR_OFFSET_UP_DOWN, val);
	}

	uint8_t FT6336UDriver::read_distance_left_right() {
		return readByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT);
	}

	void FT6336UDriver::write_distance_left_right(uint8_t val) {
		writeByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT, val);
	}

	uint8_t FT6336UDriver::read_distance_up_down() {
		return readByte(FT6336U_ADDR_DISTANCE_UP_DOWN);
	}

	void FT6336UDriver::write_distance_up_down(uint8_t val) {
		writeByte(FT6336U_ADDR_DISTANCE_UP_DOWN, val);
	}

	uint8_t FT6336UDriver::read_distance_zoom() {
		return readByte(FT6336U_ADDR_DISTANCE_ZOOM);
	}

	void FT6336UDriver::write_distance_zoom(uint8_t val) {
		writeByte(FT6336U_ADDR_DISTANCE_ZOOM, val);
	}

// System Information
	uint16_t FT6336UDriver::read_library_version() {
		uint8_t buffer[2];
		buffer[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
		buffer[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint8_t FT6336UDriver::read_chip_id() {
		return readByte(FT6336U_ADDR_CHIP_ID);
	}

	uint8_t FT6336UDriver::read_g_mode() {
		return readByte(FT6336U_ADDR_G_MODE);
	}

	void FT6336UDriver::write_g_mode(G_MODE_Enum mode) {
		writeByte(FT6336U_ADDR_G_MODE, mode);
	}

	uint8_t FT6336UDriver::read_pwrmode() {
		return readByte(FT6336U_ADDR_POWER_MODE);
	}

	uint8_t FT6336UDriver::read_firmware_id() {
		return readByte(FT6336U_ADDR_FIRMARE_ID);
	}

	uint8_t FT6336UDriver::read_focaltech_id() {
		return readByte(FT6336U_ADDR_FOCALTECH_ID);
	}

	uint8_t FT6336UDriver::read_release_code_id() {
		return readByte(FT6336U_ADDR_RELEASE_CODE_ID);
	}

	uint8_t FT6336UDriver::read_state() {
		return readByte(FT6336U_ADDR_STATE);
	}

	uint8_t FT6336UDriver::readByte(uint8_t addr) {
		uint8_t result = 0;
		uint8_t readLength;

		do {
			Wire.beginTransmission(I2C_ADDR_FT6336U);
			Wire.write(addr);
			Wire.endTransmission(false); // Restart
//        delay(10);
			readLength = Wire.requestFrom(I2C_ADDR_FT6336U, 1);
		} while (readLength == 0);

		while (Wire.available()) {
			result = Wire.read();
		}

		return result;
	}

	void FT6336UDriver::writeByte(uint8_t addr, uint8_t data) {
		Wire.beginTransmission(I2C_ADDR_FT6336U);
		Wire.write(addr);
		Wire.write(data);
		Wire.endTransmission();
	}

	// ------------------------------------------------------------------------

	Point FT6336UDriver::readRotatedPoint1(ScreenBuffer* screenBuffer) {
		auto point = Point(
			read_touch1_x(),
			read_touch1_y()
		);

		screenBuffer->getDriver()->rotatePointForOrientation(point);

		return point;
	}

	Point FT6336UDriver::readRotatedPoint2(ScreenBuffer* screenBuffer) {
		auto point = Point(
			read_touch2_x(),
			read_touch2_y()
		);

		screenBuffer->getDriver()->rotatePointForOrientation(point);

		return point;
	}

	void FT6336UDriver::tick(ScreenBuffer* screenBuffer, const std::function<void(Event&)>& callback) {
		if (!_interrupted)
			return;

		_interrupted = false;

		auto isDown1 = read_touch1_event() == 2;
		auto isDown2 = read_touch2_event() == 2;

		if (isDown1) {
			if (isDown2) {
				// Pinch drag
				if (_wasPinched) {
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
					_wasPinched = true;

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
				if (_wasPinched) {
					_wasPinched = false;

					_touchPoints[1].setDown(false);
					_touchPoints[1].setPosition(readRotatedPoint2(screenBuffer));

					auto event = PinchUpEvent(
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					);

					callback(event);
				}
				// Touch drag
				else if (_wasTouched) {
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
					_wasTouched = true;

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
			if (_wasPinched) {
				_wasPinched = false;

				_touchPoints[1].setDown(false);
				_touchPoints[1].setPosition(readRotatedPoint2(screenBuffer));

				auto event = PinchUpEvent(
					_touchPoints[0].getPosition(),
					_touchPoints[1].getPosition()
				);

				callback(event);
			}

			// Touch up
			if (_wasTouched) {
				_wasTouched = false;

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