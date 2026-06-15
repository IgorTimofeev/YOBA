#include <cstdint>
#include <YOBA/system.h>
#include "SH1106Display.h"

namespace YOBA {
	SH1106Display::SH1106Display(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequency
	) :
		RenderingTarget(
			Size(128, 64),
			Rotation::none,
			PixelOrder::XY,
			ColorModel::monochrome
		),
		SPIDisplay(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			RSTPin,
			SPIFrequency
		),
		ContrastDisplay()
	{

	}

	void SH1106Display::setup() {
		SPIDisplay::setup();

		writeCommand(static_cast<uint8_t>(Command::displayOff));
		writeCommand(static_cast<uint8_t>(Command::setDisplayClockDiv));
		writeData(0xF0); // Suggested ratio = 0xF0
		writeCommand(static_cast<uint8_t>(Command::setMultiplex));
		writeData(0x3F);
		writeCommand(static_cast<uint8_t>(Command::outputFollowsRam));
		writeCommand(static_cast<uint8_t>(Command::setDisplayOffset));
		writeData(0x0); // Without offset
		writeCommand(static_cast<uint8_t>(Command::setStartLine)); // Start line from 0, like "setStartLine | 0x0"
		writeCommand(static_cast<uint8_t>(Command::chargePump));
		writeData(0x14);
		writeCommand(static_cast<uint8_t>(Command::memoryMode));
		writeData(0x0); // 0x0 = horizontal, 0x2 = paged
		writeCommand(static_cast<uint8_t>(Command::setPageAddress));
		//		writeData(static_cast<uint8_t>(Command::segremap | 0x1)); // ?????????????
		writeCommand(static_cast<uint8_t>(Command::comScanDec));
		writeCommand(static_cast<uint8_t>(Command::setLowColumn));
		writeCommand(static_cast<uint8_t>(Command::setHighColumn));
		writeCommand(static_cast<uint8_t>(Command::setComPins));
		writeCommand(0x12);

		setContrast(0xCF);

		writeCommand(static_cast<uint8_t>(Command::setSegmentRemap));
		writeCommand(static_cast<uint8_t>(Command::setPrecharge));
		writeData(0xF1);
		writeCommand(static_cast<uint8_t>(Command::setVComDetect));
		writeData(0x20); // 0.77xVcc
		writeCommand(static_cast<uint8_t>(Command::displayAllOnResume));

		setInverted(false);

		writeCommand(static_cast<uint8_t>(Command::displayOn));
	}

	void SH1106Display::writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		for (uint8_t page = 0; page < _pageCount; page++) {
			writeCommand(static_cast<uint8_t>(Command::setPageAddress) | page);
			writeCommand(static_cast<uint8_t>(Command::setColumnAddressLow) | 0);
			writeCommand(static_cast<uint8_t>(Command::setColumnAddressHigh) | 0);

			// Pixels
			writeData({ pixelBuffer.data() + page * getSize().getWidth(), getSize().getWidth() });
		}
	}

	void SH1106Display::setContrast(const uint8_t value) {
		writeCommand(static_cast<uint8_t>(Command::setContrast));
		writeData(value);
	}

	void SH1106Display::setInverted(const bool value) {
		writeCommand(static_cast<uint8_t>(value ? Command::invertDisplay : Command::normalDisplay));
	}
}