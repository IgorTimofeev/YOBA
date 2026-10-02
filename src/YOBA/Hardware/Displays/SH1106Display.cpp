#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/SH1106Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void SH1106Display::setup(SPIDisplayInterface* displayInterface) {
		SPIDisplay::setup(
			displayInterface,

			Size(128, 64),
			Rotation::none,
			PixelOrder::XNormalYNormal,
			ColorModel::monochrome
		);

		// Reset pin
		if (_interface->getRSTPin() >= 0)
			_interface->toggleRSTPin(100, 100);

		_interface->writeCommand(static_cast<uint8_t>(Command::displayOff));
		_interface->writeCommand(static_cast<uint8_t>(Command::setDisplayClockDiv));
		_interface->write(0xF0); // Suggested ratio = 0xF0
		_interface->writeCommand(static_cast<uint8_t>(Command::setMultiplex));
		_interface->write(0x3F);
		_interface->writeCommand(static_cast<uint8_t>(Command::outputFollowsRam));
		_interface->writeCommand(static_cast<uint8_t>(Command::setDisplayOffset));
		_interface->write(0x0); // Without offset
		_interface->writeCommand(static_cast<uint8_t>(Command::setStartLine)); // Start line from 0, like "setStartLine | 0x0"
		_interface->writeCommand(static_cast<uint8_t>(Command::chargePump));
		_interface->write(0x14);
		_interface->writeCommand(static_cast<uint8_t>(Command::memoryMode));
		_interface->write(0x0); // 0x0 = horizontal, 0x2 = paged
		_interface->writeCommand(static_cast<uint8_t>(Command::setPageAddress));
		//		_SPIDevice.write(static_cast<uint8_t>(Command::segremap | 0x1)); // ?????????????
		_interface->writeCommand(static_cast<uint8_t>(Command::comScanDec));
		_interface->writeCommand(static_cast<uint8_t>(Command::setLowColumn));
		_interface->writeCommand(static_cast<uint8_t>(Command::setHighColumn));
		_interface->writeCommand(static_cast<uint8_t>(Command::setComPins));
		_interface->writeCommand(0x12);

		setContrast(0xCF);

		_interface->writeCommand(static_cast<uint8_t>(Command::setSegmentRemap));
		_interface->writeCommand(static_cast<uint8_t>(Command::setPrecharge));
		_interface->write(0xF1);
		_interface->writeCommand(static_cast<uint8_t>(Command::setVComDetect));
		_interface->write(0x20); // 0.77xVcc
		_interface->writeCommand(static_cast<uint8_t>(Command::displayAllOnResume));

		setInverted(false);

		_interface->writeCommand(static_cast<uint8_t>(Command::displayOn));
	}

	void SH1106Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		for (uint8_t page = 0; page < _pageCount; page++) {
			_interface->writeCommand(static_cast<uint8_t>(Command::setPageAddress) | page);
			_interface->writeCommand(static_cast<uint8_t>(Command::setColumnAddressLow) | 0);
			_interface->writeCommand(static_cast<uint8_t>(Command::setColumnAddressHigh) | 0);

			// Pixels
			_interface->write({ pixelBuffer.data() + page * getSize().getWidth(), getSize().getWidth() });
		}
	}

	void SH1106Display::setContrast(const uint8_t value) {
		_interface->writeCommand(static_cast<uint8_t>(Command::setContrast));
		_interface->write(value);
	}

	void SH1106Display::setInverted(const bool value) {
		_interface->writeCommand(static_cast<uint8_t>(value ? Command::invertDisplay : Command::normalDisplay));
	}
}

#endif