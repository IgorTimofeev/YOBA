/**************************************************************************/
/*!
  @file     FT6336U.cpp
  Author: Atsushi Sasaki (https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_MCU)

#include <YOBA/System.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>

#include <YOBA/Hardware/TouchPanels/FT6336UTouchPanel.hpp>

namespace YOBA {
	void FT6336UTouchPanel::setup(
		const uint8_t SDAPin,
		const uint8_t SCLPin,
		const int8_t RSTPin,
		const uint8_t INTPin,

		const uint8_t I2CAddress,
		const uint32_t I2CFrequencyHz
	) {
		_I2CDevice.setup(
			SDAPin,
			SCLPin,
			I2CAddress,
			I2CFrequencyHz
		);

		// Interrupt
		_INTPin = INTPin;

		system::GPIO::setMode(_INTPin, system::GPIO::PinMode::input);
		system::GPIO::addInterruptHandler(_INTPin, interruptHandler, this);

		// Reset
		_RSTPin = RSTPin;

		if (_RSTPin >= 0) {
			system::GPIO::setMode(_RSTPin, system::GPIO::PinMode::output);
			system::GPIO::write(_RSTPin, false);
			system::delayMs(10);
			system::GPIO::write(_RSTPin, true);
		}

		// Do we need some delay? Hmmm
		// system::sleep(500);
	}

	FT6336UDeviceMode FT6336UTouchPanel::readDeviceMode() const {
		const auto value = (readByte(ADDR_DEVICE_MODE) & 0x70) >> 4;

		return value == 0b000 ? FT6336UDeviceMode::workingMode : FT6336UDeviceMode::factoryMode;
	}

	void FT6336UTouchPanel::writeDeviceMode(const FT6336UDeviceMode mode) const {
		const auto value = mode == FT6336UDeviceMode::workingMode ? 0b000 : 0b100;

		writeByte(ADDR_DEVICE_MODE, (value & 0x07) << 4);
	}

	uint8_t FT6336UTouchPanel::readGestureID() const {
		return readByte(ADDR_GESTURE_ID);
	}

	uint8_t FT6336UTouchPanel::readTDStatus() const {
		return readByte(ADDR_TD_STATUS);
	}

	uint8_t FT6336UTouchPanel::readTouchNumber() const {
		return readByte(ADDR_TD_STATUS) & 0x0F;
	}

// Touch 1 functions
	uint16_t FT6336UTouchPanel::readTouch1X() const {
		uint8_t buffer[2];
		buffer[0] = readByte(ADDR_TOUCH1_X);
		buffer[1] = readByte(ADDR_TOUCH1_X + 1);

		return (buffer[0] & 0x0f) << 8 | buffer[1];
	}

	uint16_t FT6336UTouchPanel::readTouch1Y() const {
		uint8_t buffer[2];
		buffer[0] = readByte(ADDR_TOUCH1_Y);
		buffer[1] = readByte(ADDR_TOUCH1_Y + 1);

		return (buffer[0] & 0x0f) << 8 | buffer[1];
	}

	uint8_t FT6336UTouchPanel::readTouch1Event() const {
		return readByte(ADDR_TOUCH1_EVENT) >> 6;
	}

	uint8_t FT6336UTouchPanel::readTouch1ID() const {
		return readByte(ADDR_TOUCH1_ID) >> 4;
	}

	uint8_t FT6336UTouchPanel::readTouch1Weight() const {
		return readByte(ADDR_TOUCH1_WEIGHT);
	}

	uint8_t FT6336UTouchPanel::readTouch1Misc() const {
		return readByte(ADDR_TOUCH1_MISC) >> 4;
	}

// Touch 2 functions
	uint16_t FT6336UTouchPanel::readTouch2X() const {
		uint8_t buffer[2];
		buffer[0] = readByte(ADDR_TOUCH2_X);
		buffer[1] = readByte(ADDR_TOUCH2_X + 1);

		return (buffer[0] & 0x0f) << 8 | buffer[1];
	}

	uint16_t FT6336UTouchPanel::readTouch2Y() const {
		uint8_t buffer[2];
		buffer[0] = readByte(ADDR_TOUCH2_Y);
		buffer[1] = readByte(ADDR_TOUCH2_Y + 1);

		return (buffer[0] & 0x0f) << 8 | buffer[1];
	}

	uint8_t FT6336UTouchPanel::readTouch2Event() const {
		return readByte(ADDR_TOUCH2_EVENT) >> 6;
	}

	uint8_t FT6336UTouchPanel::readTouch2ID() const {
		return readByte(ADDR_TOUCH2_ID) >> 4;
	}

	uint8_t FT6336UTouchPanel::readTouch2Weight() const {
		return readByte(ADDR_TOUCH2_WEIGHT);
	}

	uint8_t FT6336UTouchPanel::readTouch2Misc() const {
		return readByte(ADDR_TOUCH2_MISC) >> 4;
	}

// Mode Parameter Register
	uint8_t FT6336UTouchPanel::readTouchThreshold() const {
		return readByte(ADDR_THRESHOLD);
	}

	uint8_t FT6336UTouchPanel::readFilterCoefficient() const {
		return readByte(ADDR_FILTER_COE);
	}

	FT6336UCtrlMode FT6336UTouchPanel::readCtrlMode() const {
		return readByte(ADDR_CTRL) == 0 ? FT6336UCtrlMode::keepActiveMode : FT6336UCtrlMode::switchToMonitorMode;
	}

	void FT6336UTouchPanel::writeCtrlMode(const FT6336UCtrlMode mode) const {
		writeByte(ADDR_CTRL, mode == FT6336UCtrlMode::keepActiveMode ? 0 : 1);
	}

	uint8_t FT6336UTouchPanel::readTimePeriodEnterMonitor() const {
		return readByte(ADDR_TIME_ENTER_MONITOR);
	}

	uint8_t FT6336UTouchPanel::readActiveRate() const {
		return readByte(ADDR_ACTIVE_MODE_RATE);
	}

	uint8_t FT6336UTouchPanel::readMonitorRate() const {
		return readByte(ADDR_MONITOR_MODE_RATE);
	}

// Gesture Parameters
	uint8_t FT6336UTouchPanel::readRadianValue() const {
		return readByte(ADDR_RADIAN_VALUE);
	}

	void FT6336UTouchPanel::writeRadianValue(const uint8_t val) const {
		writeByte(ADDR_RADIAN_VALUE, val);
	}

	uint8_t FT6336UTouchPanel::readOffsetLeftRight() const {
		return readByte(ADDR_OFFSET_LEFT_RIGHT);
	}

	void FT6336UTouchPanel::writeOffsetLeftRight(const uint8_t val) const {
		writeByte(ADDR_OFFSET_LEFT_RIGHT, val);
	}

	uint8_t FT6336UTouchPanel::readOffsetUpDown() const {
		return readByte(ADDR_OFFSET_UP_DOWN);
	}

	void FT6336UTouchPanel::writeOffsetUpDown(const uint8_t val) const {
		writeByte(ADDR_OFFSET_UP_DOWN, val);
	}

	uint8_t FT6336UTouchPanel::readDistanceLeftRight() const {
		return readByte(ADDR_DISTANCE_LEFT_RIGHT);
	}

	void FT6336UTouchPanel::writeDistanceLeftRight(const uint8_t val) const {
		writeByte(ADDR_DISTANCE_LEFT_RIGHT, val);
	}

	uint8_t FT6336UTouchPanel::readDistanceUpDown() const {
		return readByte(ADDR_DISTANCE_UP_DOWN);
	}

	void FT6336UTouchPanel::writeDistanceUpDown(const uint8_t val) const {
		writeByte(ADDR_DISTANCE_UP_DOWN, val);
	}

	uint8_t FT6336UTouchPanel::readDistanceZoom() const {
		return readByte(ADDR_DISTANCE_ZOOM);
	}

	void FT6336UTouchPanel::writeDistanceZoom(const uint8_t val) const {
		writeByte(ADDR_DISTANCE_ZOOM, val);
	}

// System Information
	uint16_t FT6336UTouchPanel::readLibraryVersion() const {
		uint8_t buffer[2];
		buffer[0] = readByte(ADDR_LIBRARY_VERSION_H);
		buffer[1] = readByte(ADDR_LIBRARY_VERSION_L);

		return ((buffer[0] & 0x0f) << 8) | buffer[1];
	}

	uint8_t FT6336UTouchPanel::readChipID() const {
		return readByte(ADDR_CHIP_ID);
	}

	FT6336UGMode FT6336UTouchPanel::readGMode() const {
		return readByte(ADDR_G_MODE) == 0 ? FT6336UGMode::pollingMode : FT6336UGMode::triggerMode;
	}

	void FT6336UTouchPanel::writeGMode(const FT6336UGMode mode) const {
		writeByte(ADDR_G_MODE, mode == FT6336UGMode::pollingMode ? 0 : 1);
	}

	uint8_t FT6336UTouchPanel::readPWRMode() const {
		return readByte(ADDR_POWER_MODE);
	}

	uint8_t FT6336UTouchPanel::readFirmwareID() const {
		return readByte(ADDR_FIRMARE_ID);
	}

	uint8_t FT6336UTouchPanel::readFocaltechID() const {
		return readByte(ADDR_FOCALTECH_ID);
	}

	uint8_t FT6336UTouchPanel::readReleaseCodeID() const {
		return readByte(ADDR_RELEASE_CODE_ID);
	}

	uint8_t FT6336UTouchPanel::readState() const {
		return readByte(ADDR_STATE);
	}

	uint8_t FT6336UTouchPanel::readByte(const uint8_t addr) const {
		uint8_t buffer = addr;

		_I2CDevice.write({ &buffer, 1 });
		_I2CDevice.read({ &buffer, 1 });

		return buffer;
	}

	void FT6336UTouchPanel::writeByte(const uint8_t addr, const uint8_t data) const {
		const uint8_t buffer[2] {
			addr,
			data
		};

		_I2CDevice.write({ buffer, 2 });
	}

	// ------------------------------------------------------------------------

	void FT6336UTouchPanel::interruptHandler(void* args) {
		static_cast<FT6336UTouchPanel*>(args)->_interrupted = true;
	}

	Point FT6336UTouchPanel::readOrientedPoint1(const RenderingTarget* target) const {
		return target->applyRotation(Point(
			readTouch1X(),
			readTouch1Y()
		));
	}

	Point FT6336UTouchPanel::readOrientedPoint2(const RenderingTarget* target) const {
		return target->applyRotation(Point(
			readTouch2X(),
			readTouch2Y()
		));
	}

	void FT6336UTouchPanel::tick() {
		if (!_interrupted)
			return;

		_interrupted = false;

		const auto application = Application::getCurrent();
		const auto renderingTarget = application->getRenderer()->getTarget();

		const auto isDown1 = readTouch1Event() == 2;
		const auto isDown2 = readTouch2Event() == 2;

		if (isDown1) {
			if (isDown2) {
				// Pinch drag
				if (_wasPinched) {
					const auto& point1 = readOrientedPoint1(renderingTarget);
					const auto& point2 = readOrientedPoint2(renderingTarget);

					if (
						point1 != _touchPoints[0].getPosition()
						|| point2 != _touchPoints[1].getPosition()
					) {

						_touchPoints[0].setPosition(point1);
						_touchPoints[1].setPosition(point2);

						PinchDragEvent event {
							_touchPoints[0].getPosition(),
							_touchPoints[1].getPosition()
						};

						application->pushEvent(&event);
					}
				}
				// Pinch down
				else {
					_wasPinched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readOrientedPoint1(renderingTarget));

					_touchPoints[1].setDown(true);
					_touchPoints[1].setPosition(readOrientedPoint2(renderingTarget));

					PinchDownEvent event {
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					;

					application->pushEvent(&event);
				}
			}
			else {
				// Pinch up
				if (_wasPinched) {
					_wasPinched = false;

					_touchPoints[1].setDown(false);
					_touchPoints[1].setPosition(readOrientedPoint2(renderingTarget));

					PinchUpEvent event {
						_touchPoints[0].getPosition(),
						_touchPoints[1].getPosition()
					};

					application->pushEvent(&event);
				}
				// Touch drag
				else if (_wasTouched) {
					const auto& point1 = readOrientedPoint1(renderingTarget);

					if (point1 != _touchPoints[0].getPosition()) {
						_touchPoints[0].setPosition(point1);

						PointerDragEvent event {
							_touchPoints[0].getPosition()
						};

						application->pushEvent(&event);
					}
				}
				// Touch down
				else {
					_wasTouched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readOrientedPoint1(renderingTarget));

					PointerDownEvent event {
						_touchPoints[0].getPosition()
					};

					application->pushEvent(&event);
				}
			}
		}
		else {
			// Pinch up
			if (_wasPinched) {
				_wasPinched = false;

				_touchPoints[1].setDown(false);
				_touchPoints[1].setPosition(readOrientedPoint2(renderingTarget));

				PinchUpEvent event {
					_touchPoints[0].getPosition(),
					_touchPoints[1].getPosition()
				};

				application->pushEvent(&event);
			}

			// Touch up
			if (_wasTouched) {
				_wasTouched = false;

				_touchPoints[0].setDown(false);
				_touchPoints[0].setPosition(readOrientedPoint1(renderingTarget));

				PointerUpEvent event {
					_touchPoints[0].getPosition()
				};

				application->pushEvent(&event);
			}
		}
	}
}

#endif