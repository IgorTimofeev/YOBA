#include "ST7565Display.h"
#include <YOBA/system.h>
#include <cstdint>

namespace YOBA {
	ST7565Display::ST7565Display(
		uint8_t mosiPin,
		uint8_t misoPin,
		uint8_t sckPin,
		uint8_t ssPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		RenderTarget(
			Size(128, 64),
			ViewportRotation::clockwise0,
			PixelOrder::XYReversed,
			ColorModel::monochrome
		),
		SPIDisplay(
			mosiPin,
			misoPin,
			sckPin,
			ssPin,
			dcPin,
			rstPin,
			SPIFrequency
		),
		ContrastDisplay()
	{

	}

	void ST7565Display::setup() {
		SPIDisplay::setup();

		// LCD bias select
		writeCommand(static_cast<uint8_t>(Command::SET_BIAS_7));
		// ADC select
		writeCommand(static_cast<uint8_t>(Command::SET_ADC_NORMAL));
		// SHL select
		writeCommand(static_cast<uint8_t>(Command::SET_COM_NORMAL));
		// Initial display line
		writeCommand(static_cast<uint8_t>(Command::SET_DISP_START_LINE));

		// turn on voltage converter (VC=1, VR=0, VF=0)
		writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x4);
		// wait for 50% rising
		system::sleep(50'000);

		// turn on voltage regulator (VC=1, VR=1, VF=0)
		writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x6);
		// wait >=50ms
		system::sleep(50'000);

		// turn on voltage follower (VC=1, VR=1, VF=1)
		writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x7);
		// wait
		system::sleep(10'000);

		// set lcd operating voltage (regulator resistor, ref voltage resistor)
		writeCommand(static_cast<uint8_t>(Command::SET_RESISTOR_RATIO) | 0x6);

		writeCommand(static_cast<uint8_t>(Command::DISPLAY_ON));
		writeCommand(static_cast<uint8_t>(Command::SET_ALLPTS_NORMAL));
		setContrast(0x9);
	}

	void ST7565Display::writePixels(const Bounds& bounds, uint8_t* source, size_t length) {
//		for (uint8_t page = 0; page < pageCount; page++) {
//			writeCommand(static_cast<uint8_t>(Command::SetPageAddress) | page);
//			writeCommand(static_cast<uint8_t>(Command::SetColumnAddressLow));
//			writeCommand(static_cast<uint8_t>(Command::SetColumnAddressHigh));
//
//			// Page
//			writeData(buffer + page * getResolution().getWidth(), getResolution().getWidth());
//		}

		for (uint8_t p = 0; p <= 7; p++) {
			uint8_t col = 0;

			writeCommand(static_cast<uint8_t>(Command::SET_PAGE) | (7 - p));

			writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_LOWER) | ((col) & 0xf));
			writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_UPPER) | (((col) >> 4) & 0x0F));
			writeCommand(static_cast<uint8_t>(Command::RMW));

			for (; col < static_cast<uint8_t>(getSize().getWidth()); col++) {
				writeData(source[getSize().getWidth() * p + col]);
			}
		}
	}

	void ST7565Display::setContrast(uint8_t value) {
		writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_FIRST));
		writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_SECOND) | (value & 0x3f));
	}

	void ST7565Display::setInverted(bool value) {
		InvertibleDisplay::setInverted(value);

		writeCommand(static_cast<uint8_t>(value ? Command::SET_DISP_REVERSE : Command::SET_DISP_NORMAL));
	}
}