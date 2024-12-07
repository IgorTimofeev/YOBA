#include "Arduino.h"
#include <SPI.h>
#include "SPIScreenDriver.h"

namespace yoba {
	SPIScreenDriver::SPIScreenDriver(
		ScreenDriverBufferType bufferType,
		ColorModel colorModel,
		const Size& resolution,
		ScreenOrientation orientation,

		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		ScreenDriver(
			bufferType,
			colorModel,
			resolution,
			orientation
		),
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_spiSettings(SPISettings(SPIFrequency, SPI_MSBFIRST, SPI_MODE0))
	{

	}

	void SPIScreenDriver::setup() {
		ScreenDriver::setup();

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

	void SPIScreenDriver::writeData(uint8_t data) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transfer(data);
		SPI.endTransaction();

		setChipSelect(true);
	}

	void SPIScreenDriver::writeData(const uint8_t* data, size_t length) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transferBytes(data, nullptr, length);
		SPI.endTransaction();

		setChipSelect(true);
	}

	void SPIScreenDriver::writeCommand(uint8_t data) {
		setDataCommand(false);
		writeData(data);
		setDataCommand(true);
	}

	void SPIScreenDriver::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	void SPIScreenDriver::writeCommandAndData(uint8_t command, const uint8_t *data, size_t length) {
		writeCommand(command);
		writeData(data, length);
	}

	void SPIScreenDriver::onOrientationChanged() {
		ScreenDriver::onOrientationChanged();

		writeOrientationChangeCommands();
	}

	void SPIScreenDriver::setChipSelect(uint8_t value) const {
		digitalWrite(_csPin, value);
	}

	void SPIScreenDriver::setDataCommand(uint8_t value) const {
		digitalWrite(_dcPin, value);
	}
}