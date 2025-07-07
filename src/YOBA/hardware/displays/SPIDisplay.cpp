#include "SPIDisplay.h"
#include <YOBA/system.h>

namespace YOBA {
	SPIDisplay::SPIDisplay(
		const uint8_t mosiPin,
		const uint8_t misoPin,
		const uint8_t sckPin,
		const int8_t ssPin,
		const uint8_t dcPin,
		const int8_t rstPin,
		const uint32_t frequency
	) :
		mosiPin(mosiPin),
		misoPin(misoPin),
		sckPin(sckPin),
		ssPin(ssPin),
		dcPin(dcPin),
		rstPin(rstPin),
		frequency(frequency)
	{

	}

	void SPIDisplay::setup() {
		RenderTarget::setup();

		// Non-SPI GPIO

		// Data/command pin
		system::GPIO::setMode(dcPin, system::GPIO::PinMode::output);
		setDataCommandPin(true);

		// Reset pin
		if (rstPin >= 0) {
			system::GPIO::setMode(rstPin, system::GPIO::PinMode::output);

			toggleResetPin();
		}

		// SPI
		system::SPI::setup(mosiPin, sckPin, ssPin, frequency);
	}

	void SPIDisplay::writeData(const uint8_t data) {
		system::SPI::write(data);
	}

	void SPIDisplay::writeData(const uint8_t* data, const size_t length) {
		system::SPI::write(data, length);
	}

	void SPIDisplay::writeCommand(const uint8_t command) {
		setDataCommandPin(false);
		writeData(command);
		setDataCommandPin(true);
	}

	void SPIDisplay::writeCommandAndData(const uint8_t command, const uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	void SPIDisplay::writeCommandAndData(const uint8_t command, const uint8_t *data, const size_t length) {
		writeCommand(command);
		writeData(data, length);
	}

	void SPIDisplay::setDataCommandPin(const bool value) const {
		system::GPIO::write(dcPin, value);
	}

	void SPIDisplay::setResetPin(const bool value) const {
		system::GPIO::write(rstPin, value);
	}

	void SPIDisplay::toggleResetPin() {
		setResetPin(false);
		system::sleep(100'000);

		setResetPin(true);
		system::sleep(100'000);
	}
}