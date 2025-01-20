#include <cstdint>
#include "ST7565Display.h"

namespace yoba::hardware {
	ST7565Display::ST7565Display(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		Display(
			Size(128, 64),
			RenderTargetOrientation::clockwise0,
			ColorModel::monochrome,
			RenderTargetPixelWriting::direct,
			RenderTargetPixelOrder::XYReversed
		),
		SPIDisplay(
			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		),
		DirectRenderTarget(),
		ContrastDisplay()
	{

	}

	void ST7565Display::writeOrientationChangeCommands() {

	}

	void ST7565Display::writeColorModeChangeCommands() {

	}

	void ST7565Display::writeSetupCommands() {
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
		system::sleep(50);

		// turn on voltage regulator (VC=1, VR=1, VF=0)
		writeCommand((uint8_t) Command::SET_POWER_CONTROL | 0x6);
		// wait >=50ms
		system::sleep(50);

		// turn on voltage follower (VC=1, VR=1, VF=1)
		writeCommand((uint8_t) Command::SET_POWER_CONTROL | 0x7);
		// wait
		system::sleep(10);

		// set lcd operating voltage (regulator resistor, ref voltage resistor)
		writeCommand((uint8_t) Command::SET_RESISTOR_RATIO | 0x6);

		writeCommand((uint8_t) Command::DISPLAY_ON);
		writeCommand((uint8_t) Command::SET_ALLPTS_NORMAL);
		setContrast(0x9);
	}

	void ST7565Display::writePixels(uint8_t* buffer) {
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

			for (; col < (uint8_t) getSize().getWidth(); col++) {
				writeData(buffer[(getSize().getWidth() * p) + col]);
			}
		}
	}

	void ST7565Display::setContrast(uint8_t value) {
		writeCommand((uint8_t) Command::SET_VOLUME_FIRST);
		writeCommand((uint8_t) Command::SET_VOLUME_SECOND | (value & 0x3f));
	}

	void ST7565Display::setInverted(bool value) {
		InvertibleDisplay::setInverted(value);

		writeCommand((uint8_t) (value ? Command::SET_DISP_REVERSE : Command::SET_DISP_NORMAL));
	}
}