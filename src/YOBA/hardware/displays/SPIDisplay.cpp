#include "SPIDisplay.h"
#include <YOBA/system.h>

namespace YOBA {
	SPIDisplay::SPIDisplay(
		const uint8_t mosiPin,
		const uint8_t sckPin,
		const int8_t ssPin,
		const uint8_t dcPin,
		const int8_t rstPin,
		const uint32_t frequencyHz
	) :
		_SPIDevice(
			mosiPin,
			sckPin,
			ssPin,
			dcPin,
			frequencyHz
		),
		_rstPin(rstPin)
	{

	}

	void SPIDisplay::setup() {
		RenderingTarget::setup();

		// Reset pin
		if (_rstPin >= 0) {
			system::GPIO::setMode(_rstPin, system::GPIO::pinMode::output);

			toggleResetPin();
		}

		// SPI
		_SPIDevice.setup();
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
		system::GPIO::write(_rstPin, value);
	}

	void SPIDisplay::toggleResetPin() {
		setResetPin(false);
		system::delayMs(100);

		setResetPin(true);
		system::delayMs(100);
	}
}