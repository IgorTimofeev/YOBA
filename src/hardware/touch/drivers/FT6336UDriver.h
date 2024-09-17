/**************************************************************************/
/*!
  @file     FT6336U.h
  Author: Atsushi Sasaki(https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#pragma once

#include <cstdint>
#include <Arduino.h>
#include "touchDriver.h"

namespace yoba {
	#define I2C_ADDR_FT6336U 0x38

	// Touch Parameter
	#define FT6336U_PRES_DOWN 0x2
	#define FT6336U_COORD_UD  0x1

	// Registers
	#define FT6336U_ADDR_DEVICE_MODE 	0x00
	typedef enum {
		working_mode = 0b000,
		factory_mode = 0b100,
	} DEVICE_MODE_Enum;
	#define FT6336U_ADDR_GESTURE_ID     0x01
	#define FT6336U_ADDR_TD_STATUS 		0x02

	#define FT6336U_ADDR_TOUCH1_EVENT 	0x03
	#define FT6336U_ADDR_TOUCH1_ID 		0x05
	#define FT6336U_ADDR_TOUCH1_X 		0x03
	#define FT6336U_ADDR_TOUCH1_Y 		0x05
	#define FT6336U_ADDR_TOUCH1_WEIGHT  0x07
	#define FT6336U_ADDR_TOUCH1_MISC    0x08

	#define FT6336U_ADDR_TOUCH2_EVENT 	0x09
	#define FT6336U_ADDR_TOUCH2_ID 		0x0B
	#define FT6336U_ADDR_TOUCH2_X 		0x09
	#define FT6336U_ADDR_TOUCH2_Y 		0x0B
	#define FT6336U_ADDR_TOUCH2_WEIGHT  0x0D
	#define FT6336U_ADDR_TOUCH2_MISC    0x0E

	#define FT6336U_ADDR_THRESHOLD          0x80
	#define FT6336U_ADDR_FILTER_COE         0x85
	#define FT6336U_ADDR_CTRL               0x86
	typedef enum {
		keep_active_mode = 0,
		switch_to_monitor_mode = 1,
	} CTRL_MODE_Enum;
	#define FT6336U_ADDR_TIME_ENTER_MONITOR 0x87
	#define FT6336U_ADDR_ACTIVE_MODE_RATE   0x88
	#define FT6336U_ADDR_MONITOR_MODE_RATE  0x89

	#define FT6336U_ADDR_RADIAN_VALUE           0x91
	#define FT6336U_ADDR_OFFSET_LEFT_RIGHT      0x92
	#define FT6336U_ADDR_OFFSET_UP_DOWN         0x93
	#define FT6336U_ADDR_DISTANCE_LEFT_RIGHT    0x94
	#define FT6336U_ADDR_DISTANCE_UP_DOWN       0x95
	#define FT6336U_ADDR_DISTANCE_ZOOM          0x96

	#define FT6336U_ADDR_LIBRARY_VERSION_H  0xA1
	#define FT6336U_ADDR_LIBRARY_VERSION_L  0xA2
	#define FT6336U_ADDR_CHIP_ID            0xA3
	#define FT6336U_ADDR_G_MODE             0xA4
	typedef enum {
		pollingMode = 0,
		triggerMode = 1,
	} G_MODE_Enum;
	#define FT6336U_ADDR_POWER_MODE         0xA5
	#define FT6336U_ADDR_FIRMARE_ID         0xA6
	#define FT6336U_ADDR_FOCALTECH_ID       0xA8
	#define FT6336U_ADDR_RELEASE_CODE_ID    0xAF
	#define FT6336U_ADDR_STATE              0xBC


	/**************************************************************************/
	/*!
		@brief  FT6336U I2C CTP controller driver
	*/
	/**************************************************************************/
	class FT6336UDriver : public TouchDriver {
		public:
			FT6336UDriver(uint8_t rstPin, uint8_t intPin, uint8_t sdaPin = SDA, uint8_t sclPin = SCL);

			void begin() override;
			void tick(ScreenBuffer *screenBuffer, const std::function<void(Event &)> &callback) override;

			uint8_t read_device_mode();
			void write_device_mode(DEVICE_MODE_Enum);
			uint8_t read_gesture_id();
			uint8_t read_td_status();
			uint8_t read_touch_number();
			uint16_t read_touch1_x();
			uint16_t read_touch1_y();
			uint8_t read_touch1_event();
			uint8_t read_touch1_id();
			uint8_t read_touch1_weight();
			uint8_t read_touch1_misc();
			uint16_t read_touch2_x();
			uint16_t read_touch2_y();
			uint8_t read_touch2_event();
			uint8_t read_touch2_id();
			uint8_t read_touch2_weight();
			uint8_t read_touch2_misc();

			// Mode Parameter Register
			uint8_t read_touch_threshold();
			uint8_t read_filter_coefficient();
			uint8_t read_ctrl_mode();
			void write_ctrl_mode(CTRL_MODE_Enum mode);
			uint8_t read_time_period_enter_monitor();
			uint8_t read_active_rate();
			uint8_t read_monitor_rate();

			// Gestrue Parameter Register
			uint8_t read_radian_value();
			void write_radian_value(uint8_t val);
			uint8_t read_offset_left_right();
			void write_offset_left_right(uint8_t val);
			uint8_t read_offset_up_down();
			void write_offset_up_down(uint8_t val);
			uint8_t read_distance_left_right();
			void write_distance_left_right(uint8_t val);
			uint8_t read_distance_up_down();
			void write_distance_up_down(uint8_t val);
			uint8_t read_distance_zoom();
			void write_distance_zoom(uint8_t val);

			// System Information
			uint16_t read_library_version();
			uint8_t read_chip_id();
			uint8_t read_g_mode();
			void write_g_mode(G_MODE_Enum mode);
			uint8_t read_pwrmode();
			uint8_t read_firmware_id();
			uint8_t read_focaltech_id();
			uint8_t read_release_code_id();
			uint8_t read_state();

		private:
			uint8_t _sdaPin;
			uint8_t _sclPin;
			uint8_t _rstPin;
			uint8_t _intPin;

			static uint8_t readByte(uint8_t addr);
			static void writeByte(uint8_t addr, uint8_t data);

			volatile bool _interrupted = false;
			static void onInterrupt(FT6336UDriver* driver);

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			bool _wasTouched = false;
			bool _wasPinched = false;

			Point readRotatedPoint1(ScreenBuffer* screenBuffer);
			Point readRotatedPoint2(ScreenBuffer* screenBuffer);
	};
}