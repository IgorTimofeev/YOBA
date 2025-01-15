#include "Arduino.h"
#include <SPI.h>
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
		_spiSettings(SPISettings(SPIFrequency, SPI_MSBFIRST, SPI_MODE0))
	{

	}

	void SPIDisplay::setup() {
		RenderTarget::setup();

		// Resetting CS pin just in case
		pinMode(_csPin, OUTPUT);
		setChipSelect(true);

		//Initialize non-SPI GPIOs
		pinMode(_dcPin, OUTPUT);

		// Toggle reset pin if it was defined
		if (_rstPin >= 0) {
			pinMode(_rstPin, OUTPUT);

			digitalWrite(_rstPin, 0);
			delay(100);

			digitalWrite(_rstPin, 1);
			delay(100);
		}

		// SPI
		SPI.begin();

		writeSetupCommands();
	}

	void SPIDisplay::writeData(uint8_t data) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transfer(data);
		SPI.endTransaction();

		setChipSelect(true);
	}

	void SPIDisplay::writeData(const uint8_t* data, size_t length) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transferBytes(data, nullptr, length);
		SPI.endTransaction();

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

		writeOrientationChangeCommands();
	}

	void SPIDisplay::setChipSelect(uint8_t value) const {
		digitalWrite(_csPin, value);
	}

	void SPIDisplay::setDataCommand(uint8_t value) const {
		digitalWrite(_dcPin, value);
	}
}