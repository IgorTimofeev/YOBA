#include <cstdint>
#include "SH1106Driver.h"

namespace yoba {
	SH1106Driver::SH1106Driver(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		SPIScreenDriver(
			ScreenDriverBufferType::Full,
			ColorModel::Monochrome,
			Size(128, 64),
			ScreenOrientation::Clockwise0,

			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		)
	{

	}

	void SH1106Driver::writeOrientationChangeCommands() {

	}

	void SH1106Driver::writeColorModeChangeCommands() {

	}

	void SH1106Driver::writeSetupCommands() {
		writeCommand((uint8_t) Command::DisplayOff);
		writeCommand((uint8_t) Command::SetDisplayClockDiv);
		writeCommand(0xF0); // Suggested ratio = 0xF0
		writeCommand((uint8_t) Command::SetMultiplex);
		writeCommand(0x3F);
		writeCommand((uint8_t) Command::OutputFollowsRam);
		writeCommand((uint8_t) Command::SetDisplayOffset);
		writeCommand(0x0); // Without offset
		writeCommand((uint8_t) Command::SetStartLine | 0x0); // Line 0
		writeCommand((uint8_t) Command::ChargePump);
		writeCommand(0x14);
		writeCommand((uint8_t) Command::MemoryMode);
		writeCommand(0x0); // 0x0 = horizontal, 0x2 = paged
		writeCommand((uint8_t) Command::SetPageAddress);
//		writeCommand((uint8_t) Command::Segremap | 0x1); // ?????????????
		writeCommand((uint8_t) Command::ComScanDec);
		writeCommand((uint8_t) Command::SetLowColumn);
		writeCommand((uint8_t) Command::SetHighColumn);
		writeCommand((uint8_t) Command::SetComPins);
		writeCommand(0x12);

		setContrast(0xCF);

		writeCommand((uint8_t) Command::SetSegmentRemap);
		writeCommand((uint8_t) Command::SetPrecharge);
		writeCommand(0xF1);
		writeCommand((uint8_t) Command::SetVComDetect);
		writeCommand(0x20); // 0.77xVcc
		writeCommand((uint8_t) Command::DisplayAllOnResume);
		writeCommand((uint8_t) Command::NormalDisplay);
		writeCommand((uint8_t) Command::DisplayOn);
	}

	void SH1106Driver::writePixels(uint8_t* buffer) {
		for (uint8_t page = 0; page < pageCount; page++) {
			writeCommand((uint8_t) Command::SetPageAddress + page);
			writeCommand((uint8_t) Command::SetColumnAddressLow);
			writeCommand((uint8_t) Command::SetColumnAddressHigh);

			// Page
			writeData(buffer + (page << 7), getResolution().getWidth());
		}
	}

	void SH1106Driver::setContrast(uint8_t value) {
		writeCommand((uint8_t) Command::SetContrast);
		writeCommand(0xCF);
	}
}