#include "SPIDisplay.h"

namespace yoba::hardware {
	SPIDisplay::SPIDisplay(
		uint8_t mosiPin,
		uint8_t misoPin,
		uint8_t sckPin,
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t frequency
	) :
		_mosiPin(mosiPin),
		_misoPin(misoPin),
		_sckPin(sckPin),
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_frequency(frequency)
	{

	}

	void SPIDisplay::setup() {
		RenderTarget::setup();

		// Non-SPI pins

		// Data command pin
		system::GPIO::setMode(_dcPin, system::GPIO::PinMode::Output);
		setDataCommand(true);

		// Reset pin
		if (_rstPin >= 0) {
			system::GPIO::setMode(_rstPin, system::GPIO::PinMode::Output);

			system::GPIO::write(_rstPin, false);
			system::sleep(100);

			system::GPIO::write(_rstPin, true);
			system::sleep(100);
		}

		// SPI
		system::SPI::setup(_mosiPin, _sckPin, _csPin, _frequency);

		writeSetupCommands();
	}

	void SPIDisplay::writeData(uint8_t data) {
		system::SPI::write(data);
	}

	void SPIDisplay::writeData(const uint8_t* data, size_t length) {
		system::SPI::write(data, length);
	}

	void SPIDisplay::writeCommand(uint8_t data) {
		setDataCommand(false);
		writeData(data);
		setDataCommand(true);
	}

	void SPIDisplay::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	void SPIDisplay::writeCommandAndData(uint8_t command, const uint8_t *data, size_t length) {
		writeCommand(command);
		writeData(data, length);
	}

	void SPIDisplay::onOrientationChanged() {
		RenderTarget::onOrientationChanged();

		writeOrientationChangeCommand();
	}

	void SPIDisplay::setChipSelect(uint8_t value) const {
		system::GPIO::write(_csPin, value);
	}

	void SPIDisplay::setDataCommand(uint8_t value) const {
		system::GPIO::write(_dcPin, value);
	}
}