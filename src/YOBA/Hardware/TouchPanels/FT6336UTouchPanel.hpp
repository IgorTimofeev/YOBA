/**************************************************************************/
/*!
  @file     FT6336U.h
  Author: Atsushi Sasaki(https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#pragma once

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_MCU)
#include <cstdint>

#include <YOBA/Hardware/TouchPanels/TouchPanel.hpp>
#include <YOBA/Hardware/TouchPanels/TouchPoint.hpp>

#include <YOBA/System.hpp>
#include <YOBA/Core/Vector2.hpp>
#include <YOBA/Rendering/RenderingTarget.hpp>

namespace YOBA {
	enum class FT6336UGMode : uint8_t {
		pollingMode,
		triggerMode
	};

	enum class FT6336UCtrlMode : uint8_t {
		keepActiveMode,
		switchToMonitorMode
	};

	enum class FT6336UDeviceMode : uint8_t {
		workingMode,
		factoryMode
	};

	class FT6336UTouchPanel : public TouchPanel {
		public:
			constexpr static uint8_t defaultI2CAddress = 0x38;

			void setup(
				uint8_t SDAPin,
				uint8_t SCLPin,
				int8_t RSTPin,
				uint8_t INTPin,

				uint8_t I2CAddress = defaultI2CAddress,
				uint32_t I2CFrequencyHz = 400'000
			);

			void tick() override;

			FT6336UDeviceMode readDeviceMode() const;
			void writeDeviceMode(FT6336UDeviceMode) const;
			uint8_t readGestureID() const;
			uint8_t readTDStatus() const;
			uint8_t readTouchNumber() const;
			uint16_t readTouch1X() const;
			uint16_t readTouch1Y() const;
			uint8_t readTouch1Event() const;
			uint8_t readTouch1ID() const;
			uint8_t readTouch1Weight() const;
			uint8_t readTouch1Misc() const;
			uint16_t readTouch2X() const;
			uint16_t readTouch2Y() const;
			uint8_t readTouch2Event() const;
			uint8_t readTouch2ID() const;
			uint8_t readTouch2Weight() const;
			uint8_t readTouch2Misc() const;

			// Mode Parameter Register
			uint8_t readTouchThreshold() const;
			uint8_t readFilterCoefficient() const;
			FT6336UCtrlMode readCtrlMode() const;
			void writeCtrlMode(FT6336UCtrlMode mode) const;
			uint8_t readTimePeriodEnterMonitor() const;
			uint8_t readActiveRate() const;
			uint8_t readMonitorRate() const;

			// Gesture Parameter Register
			uint8_t readRadianValue() const;
			void writeRadianValue(uint8_t val) const;
			uint8_t readOffsetLeftRight() const;
			void writeOffsetLeftRight(uint8_t val) const;
			uint8_t readOffsetUpDown() const;
			void writeOffsetUpDown(uint8_t val) const;
			uint8_t readDistanceLeftRight() const;
			void writeDistanceLeftRight(uint8_t val) const;
			uint8_t readDistanceUpDown() const;
			void writeDistanceUpDown(uint8_t val) const;
			uint8_t readDistanceZoom() const;
			void writeDistanceZoom(uint8_t val) const;

			// System Information
			uint16_t readLibraryVersion() const;
			uint8_t readChipID() const;
			FT6336UGMode readGMode() const;
			void writeGMode(FT6336UGMode mode) const;
			uint8_t readPWRMode() const;
			uint8_t readFirmwareID() const;
			uint8_t readFocaltechID() const;
			uint8_t readReleaseCodeID() const;
			uint8_t readState() const;

		private:
			system::I2CDevice _I2CDevice {};
			int8_t _RSTPin = 0;
			uint8_t _INTPin = 0;

			uint8_t readByte(uint8_t addr) const;
			void writeByte(uint8_t addr, uint8_t data) const;

			volatile bool _interrupted = false;
			static IRAM_ATTR void interruptHandler(void* args);

			TouchPoint _touchPoints[2] {
				{},
				{}
			};

			bool _wasTouched = false;
			bool _wasPinched = false;

			Point readOrientedPoint1(const RenderingTarget* target) const;
			Point readOrientedPoint2(const RenderingTarget* target) const;
			
			// Touch Parameter
			constexpr static uint8_t PRES_DOWN = 0x02;
			constexpr static uint8_t COORD_UD  = 0x01;

			// Registers
			constexpr static uint8_t ADDR_DEVICE_MODE 	= 0x00;
			constexpr static uint8_t ADDR_GESTURE_ID     = 0x01;
			constexpr static uint8_t ADDR_TD_STATUS 		= 0x02;

			constexpr static uint8_t ADDR_TOUCH1_EVENT 	= 0x03;
			constexpr static uint8_t ADDR_TOUCH1_ID 		= 0x05;
			constexpr static uint8_t ADDR_TOUCH1_X 		= 0x03;
			constexpr static uint8_t ADDR_TOUCH1_Y 		= 0x05;
			constexpr static uint8_t ADDR_TOUCH1_WEIGHT  = 0x07;
			constexpr static uint8_t ADDR_TOUCH1_MISC    = 0x08;

			constexpr static uint8_t ADDR_TOUCH2_EVENT 	= 0x09;
			constexpr static uint8_t ADDR_TOUCH2_ID 		= 0x0B;
			constexpr static uint8_t ADDR_TOUCH2_X 		= 0x09;
			constexpr static uint8_t ADDR_TOUCH2_Y 		= 0x0B;
			constexpr static uint8_t ADDR_TOUCH2_WEIGHT  = 0x0D;
			constexpr static uint8_t ADDR_TOUCH2_MISC    = 0x0E;

			constexpr static uint8_t ADDR_THRESHOLD          = 0x80;
			constexpr static uint8_t ADDR_FILTER_COE         = 0x85;
			constexpr static uint8_t ADDR_CTRL               = 0x86;
			constexpr static uint8_t ADDR_TIME_ENTER_MONITOR = 0x87;
			constexpr static uint8_t ADDR_ACTIVE_MODE_RATE   = 0x88;
			constexpr static uint8_t ADDR_MONITOR_MODE_RATE  = 0x89;

			constexpr static uint8_t ADDR_RADIAN_VALUE           = 0x91;
			constexpr static uint8_t ADDR_OFFSET_LEFT_RIGHT      = 0x92;
			constexpr static uint8_t ADDR_OFFSET_UP_DOWN         = 0x93;
			constexpr static uint8_t ADDR_DISTANCE_LEFT_RIGHT    = 0x94;
			constexpr static uint8_t ADDR_DISTANCE_UP_DOWN       = 0x95;
			constexpr static uint8_t ADDR_DISTANCE_ZOOM          = 0x96;

			constexpr static uint8_t ADDR_LIBRARY_VERSION_H  = 0xA1;
			constexpr static uint8_t ADDR_LIBRARY_VERSION_L  = 0xA2;
			constexpr static uint8_t ADDR_CHIP_ID            = 0xA3;
			constexpr static uint8_t ADDR_G_MODE             = 0xA4;

			constexpr static uint8_t ADDR_POWER_MODE         = 0xA5;
			constexpr static uint8_t ADDR_FIRMARE_ID         = 0xA6;
			constexpr static uint8_t ADDR_FOCALTECH_ID       = 0xA8;
			constexpr static uint8_t ADDR_RELEASE_CODE_ID    = 0xAF;
			constexpr static uint8_t ADDR_STATE              = 0xBC;
	};
}

#endif