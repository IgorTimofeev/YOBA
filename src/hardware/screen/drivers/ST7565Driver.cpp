#include <cstdint>
#include "ST7565Driver.h"

namespace yoba {
	ST7565Driver::ST7565Driver(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		ScreenDriver(
			ScreenPixelWritingMethod::Direct,
			ScreenPixelAlignment::XNormalYReversed,
			ColorModel::Monochrome,
			Size(128, 64),
			ScreenOrientation::Clockwise0
		),
		SPIScreenDriver(
			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		),
		DirectScreenDriver(),
		ContrastScreenDriver()
	{

	}

	void ST7565Driver::writeOrientationChangeCommands() {

	}

	void ST7565Driver::writeColorModeChangeCommands() {

	}

	void ST7565Driver::writeSetupCommands() {
		// LCD bias select
		writeCommand((uint8_t) Command::SET_BIAS_7);
		// ADC select
		writeCommand((uint8_t) Command::SET_ADC_NORMAL);
		// SHL select
		writeCommand((uint8_t) Command::SET_COM_NORMAL);
		// Initial display line
		writeCommand((uint8_t) Command::SET_DISP_START_LINE);

		// turn on voltage converter (VC=1, VR=0, VF=0)
		writeCommand((uint8_t) Command::SET_POWER_CONTROL | 0x4);
		// wait for 50% rising
		delay(50);

		// turn on voltage regulator (VC=1, VR=1, VF=0)
		writeCommand((uint8_t) Command::SET_POWER_CONTROL | 0x6);
		// wait >=50ms
		delay(50);

		// turn on voltage follower (VC=1, VR=1, VF=1)
		writeCommand((uint8_t) Command::SET_POWER_CONTROL | 0x7);
		// wait
		delay(10);

		// set lcd operating voltage (regulator resistor, ref voltage resistor)
		writeCommand((uint8_t) Command::SET_RESISTOR_RATIO | 0x6);

		writeCommand((uint8_t) Command::DISPLAY_ON);
		writeCommand((uint8_t) Command::SET_ALLPTS_NORMAL);
		setContrast(0x9);
	}

	void ST7565Driver::writePixels(uint8_t* buffer) {
//		for (uint8_t page = 0; page < pageCount; page++) {
//			writeCommand((uint8_t) Command::SetPageAddress | page);
//			writeCommand((uint8_t) Command::SetColumnAddressLow);
//			writeCommand((uint8_t) Command::SetColumnAddressHigh);
//
//			// Page
//			writeData(buffer + page * getResolution().getWidth(), getResolution().getWidth());
//		}

		uint8_t col;

		for (uint8_t p = 0; p <= 7; p++) {
			writeCommand((uint8_t) Command::SET_PAGE | (7 - p));
			col = 0;

			writeCommand((uint8_t) Command::SET_COLUMN_LOWER | ((col) & 0xf));
			writeCommand((uint8_t) Command::SET_COLUMN_UPPER | (((col) >> 4) & 0x0F));
			writeCommand((uint8_t) Command::RMW);

			for (; col < (uint8_t) getResolution().getWidth(); col++) {
				writeData(buffer[(getResolution().getWidth() * p) + col]);
			}
		}
	}

	void ST7565Driver::setContrast(uint8_t value) {
		writeCommand((uint8_t) Command::SET_VOLUME_FIRST);
		writeCommand((uint8_t) Command::SET_VOLUME_SECOND | (value & 0x3f));
	}

	void ST7565Driver::setInverted(bool value) {
		InvertibleScreenDriver::setInverted(value);

		writeCommand((uint8_t) (value ? Command::SET_DISP_REVERSE : Command::SET_DISP_NORMAL));
	}
}