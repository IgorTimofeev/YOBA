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
	FT6336UDriver::FT6336UDriver(uint8_t sdaPin, uint8_t sclPin, uint8_t rstPin, uint8_t intPin) : TouchDriver(sdaPin, sclPin, rstPin, intPin) {

	}

	void FT6336UDriver::begin() {
		TouchDriver::begin();

		// Do we need some delay? Hmmm
		//    delay(500);
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
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_X);
		read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
	}

	uint16_t FT6336UDriver::read_touch1_y() {
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
		read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
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
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_X);
		read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
	}

	uint16_t FT6336UDriver::read_touch2_y() {
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
		read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
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
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
		read_buf[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
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


//coordinate diagram（FPC downwards）
////y ////////////////////264x176
	//
	//
	//x
	//
	//
	FT6336U_TouchPointType FT6336UDriver::scan() {
		touchPoint.touch_count = read_td_status();

		if (touchPoint.touch_count == 0) {
			touchPoint.tp[0].status = release;
			touchPoint.tp[1].status = release;
		} else if (touchPoint.touch_count == 1) {
			uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
			touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
			touchPoint.tp[id1].x = read_touch1_x();
			touchPoint.tp[id1].y = read_touch1_y();
			touchPoint.tp[~id1 & 0x01].status = release;
		} else {
			uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
			touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
			touchPoint.tp[id1].x = read_touch1_x();
			touchPoint.tp[id1].y = read_touch1_y();
			uint8_t id2 = read_touch2_id(); // id2 = 0 or 1(~id1 & 0x01)
			touchPoint.tp[id2].status = (touchPoint.tp[id2].status == release) ? touch : stream;
			touchPoint.tp[id2].x = read_touch2_x();
			touchPoint.tp[id2].y = read_touch2_y();
		}

		return touchPoint;

	}

	uint8_t FT6336UDriver::readByte(uint8_t addr) {
		uint8_t rdData = 0;
		uint8_t rdDataCount;
		do {
			Wire.beginTransmission(I2C_ADDR_FT6336U);
			Wire.write(addr);
			Wire.endTransmission(false); // Restart
//        delay(10);
			rdDataCount = Wire.requestFrom(I2C_ADDR_FT6336U, 1);
		} while (rdDataCount == 0);
		while (Wire.available()) {
			rdData = Wire.read();
		}
		return rdData;

	}

	void FT6336UDriver::writeByte(uint8_t addr, uint8_t data) {
		Wire.beginTransmission(I2C_ADDR_FT6336U);
		Wire.write(addr);
		Wire.write(data);
		Wire.endTransmission();
	}

	// ------------------------------------------------------------------------

	bool FT6336UDriver::readPoint1Down() {
		return read_touch1_event() == 2;
	}

	bool FT6336UDriver::readPoint2Down() {
		return read_touch2_event() == 2;
	}

	Point FT6336UDriver::readPoint1() {
		return {
			read_touch1_x(),
			read_touch1_y()
		};
	}

	Point FT6336UDriver::readPoint2() {
		return {
			read_touch2_x(),
			read_touch2_y()
		};
	}
}