#include <cstdint>
#include "SH1106Display.h"

namespace yoba::hardware {
	SH1106Display::SH1106Display(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		RenderTarget(
			Size(128, 64),
			RenderTargetOrientation::clockwise0,
			ColorModel::monochrome,
			RenderTargetPixelWriting::direct,
			RenderTargetPixelOrder::XY
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

	void SH1106Display::writeOrientationChangeCommands() {

	}

	void SH1106Display::writeColorModeChangeCommands() {

	}

	void SH1106Display::writeSetupCommands() {
		writeCommand((uint8_t) Command::displayOff);
		writeCommand((uint8_t) Command::setDisplayClockDiv);
		writeCommand(0xF0); // Suggested ratio = 0xF0
		writeCommand((uint8_t) Command::setMultiplex);
		writeCommand(0x3F);
		writeCommand((uint8_t) Command::outputFollowsRam);
		writeCommand((uint8_t) Command::setDisplayOffset);
		writeCommand(0x0); // Without offset
		writeCommand((uint8_t) Command::setStartLine | 0x0); // Line 0
		writeCommand((uint8_t) Command::chargePump);
		writeCommand(0x14);
		writeCommand((uint8_t) Command::memoryMode);
		writeCommand(0x0); // 0x0 = horizontal, 0x2 = paged
		writeCommand((uint8_t) Command::setPageAddress);
//		writeCommand((uint8_t) Command::segremap | 0x1); // ?????????????
		writeCommand((uint8_t) Command::comScanDec);
		writeCommand((uint8_t) Command::setLowColumn);
		writeCommand((uint8_t) Command::setHighColumn);
		writeCommand((uint8_t) Command::setComPins);
		writeCommand(0x12);

		setContrast(0xCF);

		writeCommand((uint8_t) Command::setSegmentRemap);
		writeCommand((uint8_t) Command::setPrecharge);
		writeCommand(0xF1);
		writeCommand((uint8_t) Command::setVComDetect);
		writeCommand(0x20); // 0.77xVcc
		writeCommand((uint8_t) Command::displayAllOnResume);

		setInverted(false);

		writeCommand((uint8_t) Command::displayOn);
	}

	void SH1106Display::writePixels(uint8_t* buffer) {
		for (uint8_t page = 0; page < pageCount; page++) {
			writeCommand((uint8_t) Command::setPageAddress | page);
			writeCommand((uint8_t) Command::setColumnAddressLow);
			writeCommand((uint8_t) Command::setColumnAddressHigh);

			// Page
			writeData(buffer + page * getSize().getWidth(), getSize().getWidth());
		}
	}

	void SH1106Display::setContrast(uint8_t value) {
		writeCommand((uint8_t) Command::setContrast);
		writeCommand(value);
	}

	void SH1106Display::setInverted(bool value) {
		InvertibleDisplay::setInverted(value);

		writeCommand((uint8_t) (value ? Command::invertDisplay : Command::normalDisplay));
	}
}