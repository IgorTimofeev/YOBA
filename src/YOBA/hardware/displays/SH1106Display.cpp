#include <cstdint>
#include "YOBA/system.h"
#include "SH1106Display.h"

namespace YOBA {
	SH1106Display::SH1106Display(
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
			PixelOrder::XY,
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

	void SH1106Display::setup() {
		SPIDisplay::setup();

		setDataCommandPin(false);
			writeData(static_cast<uint8_t>(Command::displayOff));
			writeData(static_cast<uint8_t>(Command::setDisplayClockDiv));
			writeData(0xF0); // Suggested ratio = 0xF0
			writeData(static_cast<uint8_t>(Command::setMultiplex));
			writeData(0x3F);
			writeData(static_cast<uint8_t>(Command::outputFollowsRam));
			writeData(static_cast<uint8_t>(Command::setDisplayOffset));
			writeData(0x0); // Without offset
			writeData(static_cast<uint8_t>(Command::setStartLine)); // Start line from 0, like "setStartLine | 0x0"
			writeData(static_cast<uint8_t>(Command::chargePump));
			writeData(0x14);
			writeData(static_cast<uint8_t>(Command::memoryMode));
			writeData(0x0); // 0x0 = horizontal, 0x2 = paged
			writeData(static_cast<uint8_t>(Command::setPageAddress));
			//		writeData(static_cast<uint8_t>(Command::segremap | 0x1)); // ?????????????
			writeData(static_cast<uint8_t>(Command::comScanDec));
			writeData(static_cast<uint8_t>(Command::setLowColumn));
			writeData(static_cast<uint8_t>(Command::setHighColumn));
			writeData(static_cast<uint8_t>(Command::setComPins));
			writeData(0x12);
		setDataCommandPin(true);

		setContrast(0xCF);

		setDataCommandPin(false);
			writeData(static_cast<uint8_t>(Command::setSegmentRemap));
			writeData(static_cast<uint8_t>(Command::setPrecharge));
			writeData(0xF1);
			writeData(static_cast<uint8_t>(Command::setVComDetect));
			writeData(0x20); // 0.77xVcc
			writeData(static_cast<uint8_t>(Command::displayAllOnResume));
		setDataCommandPin(true);

		setInverted(false);

		setDataCommandPin(false);
			writeData(static_cast<uint8_t>(Command::displayOn));
		setDataCommandPin(true);
	}

	void SH1106Display::writePixels(const Bounds& bounds, uint8_t* source, size_t length) {
		for (uint8_t page = 0; page < _pageCount; page++) {
			setDataCommandPin(false);
				writeData(static_cast<uint8_t>(Command::setPageAddress) | page);
				writeData(static_cast<uint8_t>(Command::setColumnAddressLow) | 0);
				writeData(static_cast<uint8_t>(Command::setColumnAddressHigh) | 0);
			setDataCommandPin(true);

			// Pixels
			writeData(source + page * getSize().getWidth(), getSize().getWidth());
		}
	}

	void SH1106Display::setContrast(uint8_t value) {
		setDataCommandPin(false);
			writeData(static_cast<uint8_t>(Command::setContrast));
			writeData(value);
		setDataCommandPin(true);
	}

	void SH1106Display::setInverted(bool value) {
		InvertibleDisplay::setInverted(value);

		writeCommand(static_cast<uint8_t>(value ? Command::invertDisplay : Command::normalDisplay));
	}
}