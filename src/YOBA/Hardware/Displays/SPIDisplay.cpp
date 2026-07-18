#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_SPI)

#include <YOBA/Hardware/Displays/SPIDisplay.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void SPIDisplay::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequencyHz,

		const Size& size,
		const Rotation rotation,
		const PixelOrder pixelOrder,
		const ColorModel colorModel
	) {
		RenderingTarget::setup(
			size,
			rotation,
			pixelOrder,
			colorModel
		);

		// Reset pin
		_RSTPin = RSTPin;

		if (_RSTPin >= 0) {
			system::GPIO::setMode(_RSTPin, system::GPIO::PinMode::output);

			toggleResetPin();
		}

		// SPI
		_SPIDevice.setup(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			SPIFrequencyHz
		);
	}

	void SPIDisplay::writeCommand(const uint8_t command) {
		_SPIDevice.setCommandMode(true);
		_SPIDevice.write(command);
		_SPIDevice.setCommandMode(false);
	}

	void SPIDisplay::writeData(const uint8_t data) {
		_SPIDevice.write(data);
	}

	void SPIDisplay::writeData(const std::span<uint8_t> data) {
		_SPIDevice.write(data);
	}

	void SPIDisplay::setResetPin(const bool value) const {
		system::GPIO::write(_RSTPin, value);
	}

	void SPIDisplay::toggleResetPin() {
		setResetPin(false);
		system::delayMs(100);

		setResetPin(true);
		system::delayMs(100);
	}
}

#endif