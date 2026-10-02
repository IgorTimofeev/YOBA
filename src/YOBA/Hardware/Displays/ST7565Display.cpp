#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/ST7565Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void ST7565Display::setup(SPIDisplayInterface* displayInterface) {
		SPIDisplay::setup(
			displayInterface,

			Size(128, 64),
			Rotation::none,
			PixelOrder::XNormalYReversed,
			ColorModel::monochrome
		);

		// Reset pin
		if (_interface->getRSTPin() >= 0)
			_interface->toggleRSTPin(100, 100);

		// LCD bias select
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_BIAS_7));
		// ADC select
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_ADC_NORMAL));
		// SHL select
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_COM_NORMAL));
		// Initial display line
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_DISP_START_LINE));

		// turn on voltage converter (VC=1, VR=0, VF=0)
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x4);
		// wait for 50% rising
		system::delayMs(50);

		// turn on voltage regulator (VC=1, VR=1, VF=0)
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x6);
		// wait >=50ms
		system::delayMs(50);

		// turn on voltage follower (VC=1, VR=1, VF=1)
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x7);
		// wait
		system::delayMs(10);

		// set lcd operating voltage (regulator resistor, ref voltage resistor)
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_RESISTOR_RATIO) | 0x6);

		_interface->writeCommand(static_cast<uint8_t>(Command::DISPLAY_ON));
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_ALLPTS_NORMAL));
		setContrast(0x9);
	}

	void ST7565Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
//		for (uint8_t page = 0; page < pageCount; page++) {
//			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SetPageAddress) | page);
//			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SetColumnAddressLow));
//			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SetColumnAddressHigh));
//
//			// Page
//			writeData(buffer + page * getResolution().getWidth(), getResolution().getWidth());
//		}

		for (uint8_t p = 0; p <= 7; p++) {
			uint8_t col = 0;

			_interface->writeCommand(static_cast<uint8_t>(Command::SET_PAGE) | (7 - p));

			_interface->writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_LOWER) | ((col) & 0xf));
			_interface->writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_UPPER) | (((col) >> 4) & 0x0F));
			_interface->writeCommand(static_cast<uint8_t>(Command::RMW));

			for (; col < static_cast<uint8_t>(getSize().getWidth()); col++) {
				_interface->write(pixelBuffer[getSize().getWidth() * p + col]);
			}
		}
	}

	void ST7565Display::setContrast(const uint8_t value) {
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_FIRST));
		_interface->writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_SECOND) | (value & 0x3f));
	}

	void ST7565Display::setInverted(const bool value) {
		InvertibleDisplay::setInverted(value);

		_interface->writeCommand(static_cast<uint8_t>(value ? Command::SET_DISP_REVERSE : Command::SET_DISP_NORMAL));
	}
}

#endif