#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/SH1106Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void SH1106Display::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const int8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequencyHz
	) {
		SPIDisplay::setup(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			RSTPin,
			SPIFrequencyHz,

			Size(128, 64),
			Rotation::none,
			PixelOrder::XNormalYNormal,
			ColorModel::monochrome
		);

		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::displayOff));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setDisplayClockDiv));
		_SPIDevice.write(0xF0); // Suggested ratio = 0xF0
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setMultiplex));
		_SPIDevice.write(0x3F);
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::outputFollowsRam));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setDisplayOffset));
		_SPIDevice.write(0x0); // Without offset
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setStartLine)); // Start line from 0, like "setStartLine | 0x0"
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::chargePump));
		_SPIDevice.write(0x14);
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::memoryMode));
		_SPIDevice.write(0x0); // 0x0 = horizontal, 0x2 = paged
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setPageAddress));
		//		_SPIDevice.write(static_cast<uint8_t>(Command::segremap | 0x1)); // ?????????????
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::comScanDec));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setLowColumn));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setHighColumn));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setComPins));
		_SPIDevice.writeCommand(0x12);

		setContrast(0xCF);

		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setSegmentRemap));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setPrecharge));
		_SPIDevice.write(0xF1);
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setVComDetect));
		_SPIDevice.write(0x20); // 0.77xVcc
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::displayAllOnResume));

		setInverted(false);

		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::displayOn));
	}

	void SH1106Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		for (uint8_t page = 0; page < _pageCount; page++) {
			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setPageAddress) | page);
			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setColumnAddressLow) | 0);
			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setColumnAddressHigh) | 0);

			// Pixels
			_SPIDevice.write({ pixelBuffer.data() + page * getSize().getWidth(), getSize().getWidth() });
		}
	}

	void SH1106Display::setContrast(const uint8_t value) {
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::setContrast));
		_SPIDevice.write(value);
	}

	void SH1106Display::setInverted(const bool value) {
		_SPIDevice.writeCommand(static_cast<uint8_t>(value ? Command::invertDisplay : Command::normalDisplay));
	}
}

#endif