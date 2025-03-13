#include "SPIDisplay.h"

namespace yoba::hardware {
	SPIDisplay::SPIDisplay(
		uint8_t mosiPin,
		uint8_t misoPin,
		uint8_t sckPin,
		uint8_t ssPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t frequency
	) :
		_mosiPin(mosiPin),
		_misoPin(misoPin),
		_sckPin(sckPin),
		_ssPin(ssPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_frequency(frequency)
	{

	}

	SPIDisplay::~SPIDisplay() {

	}

	void SPIDisplay::setup() {
		RenderTarget::setup();

		// Non-SPI GPIO

		// Data/command pin
		system::GPIO::setMode(_dcPin, system::GPIO::PinMode::Output);
		setDataCommandPin(true);

		// Reset pin
		if (_rstPin >= 0) {
			system::GPIO::setMode(_rstPin, system::GPIO::PinMode::Output);

			toggleResetPin();
		}

		// SPI
		system::SPI::setup(_mosiPin, _sckPin, _ssPin, _frequency);
	}

	void SPIDisplay::writeData(uint8_t data) {
		system::SPI::write(data);
	}

	void SPIDisplay::writeData(const uint8_t* data, size_t length) {
		system::SPI::write(data, length);
	}

	void SPIDisplay::writeCommand(uint8_t command) {
		setDataCommandPin(false);
		writeData(command);
		setDataCommandPin(true);
	}

	void SPIDisplay::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	void SPIDisplay::writeCommandAndData(uint8_t command, const uint8_t *data, size_t length) {
		writeCommand(command);
		writeData(data, length);
	}

	void SPIDisplay::setDataCommandPin(bool value) const {
		system::GPIO::write(_dcPin, value);
	}

	void SPIDisplay::setResetPin(bool value) const {
		system::GPIO::write(_rstPin, value);
	}

	void SPIDisplay::toggleResetPin() {
		setResetPin(false);
		system::sleep(100);

		setResetPin(true);
		system::sleep(100);
	}
}