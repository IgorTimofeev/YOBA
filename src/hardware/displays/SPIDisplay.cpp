#include "SPIDisplay.h"

namespace yoba::hardware {
	SPIDisplay::SPIDisplay(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_SPIFrequency(SPIFrequency)
	{

	}

	void SPIDisplay::setup() {
		RenderTarget::setup();

		// Resetting CS pin just in case
		system::gpio::setOutput(_csPin);
		setChipSelect(true);

		// Initialize non-SPI GPIOs
		system::gpio::setOutput(_dcPin);

		// Toggle reset pin if it was defined
		if (_rstPin >= 0) {
			system::gpio::setOutput(_rstPin);

			system::gpio::write(_rstPin, 0);
			system::sleep(100);

			system::gpio::write(_rstPin, 1);
			system::sleep(100);
		}

		// SPI
		system::spi::setup();

		writeSetupCommands();
	}

	void SPIDisplay::writeData(uint8_t data) {
		setChipSelect(false);

		system::spi::writeByte(_SPIFrequency, data);

		setChipSelect(true);
	}

	void SPIDisplay::writeData(const uint8_t* data, size_t length) {
		setChipSelect(false);

		system::spi::writeBytes(_SPIFrequency, data, length);

		setChipSelect(true);
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
		system::gpio::write(_csPin, value);
	}

	void SPIDisplay::setDataCommand(uint8_t value) const {
		system::gpio::write(_dcPin, value);
	}
}