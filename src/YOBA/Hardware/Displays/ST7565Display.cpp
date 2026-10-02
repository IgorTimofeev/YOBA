#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/ST7565Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void ST7565Display::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const int8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequency
	) {
		SPIDisplay::setup(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			RSTPin,
			SPIFrequency,

			Size(128, 64),
			Rotation::none,
			PixelOrder::XNormalYReversed,
			ColorModel::monochrome
		);

		// LCD bias select
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_BIAS_7));
		// ADC select
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_ADC_NORMAL));
		// SHL select
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_COM_NORMAL));
		// Initial display line
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_DISP_START_LINE));

		// turn on voltage converter (VC=1, VR=0, VF=0)
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x4);
		// wait for 50% rising
		system::delayMs(50);

		// turn on voltage regulator (VC=1, VR=1, VF=0)
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x6);
		// wait >=50ms
		system::delayMs(50);

		// turn on voltage follower (VC=1, VR=1, VF=1)
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_POWER_CONTROL) | 0x7);
		// wait
		system::delayMs(10);

		// set lcd operating voltage (regulator resistor, ref voltage resistor)
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_RESISTOR_RATIO) | 0x6);

		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::DISPLAY_ON));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_ALLPTS_NORMAL));
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

			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_PAGE) | (7 - p));

			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_LOWER) | ((col) & 0xf));
			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_COLUMN_UPPER) | (((col) >> 4) & 0x0F));
			_SPIDevice.writeCommand(static_cast<uint8_t>(Command::RMW));

			for (; col < static_cast<uint8_t>(getSize().getWidth()); col++) {
				_SPIDevice.write(pixelBuffer[getSize().getWidth() * p + col]);
			}
		}
	}

	void ST7565Display::setContrast(const uint8_t value) {
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_FIRST));
		_SPIDevice.writeCommand(static_cast<uint8_t>(Command::SET_VOLUME_SECOND) | (value & 0x3f));
	}

	void ST7565Display::setInverted(const bool value) {
		InvertibleDisplay::setInverted(value);

		_SPIDevice.writeCommand(static_cast<uint8_t>(value ? Command::SET_DISP_REVERSE : Command::SET_DISP_NORMAL));
	}
}

#endif