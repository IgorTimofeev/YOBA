#include "SPIDisplay.h"

namespace yoba::hardware {
	SPIDisplay::SPIDisplay(
		MCUHal* hal,
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		_hal(hal),
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_SPIHalSettings(SPIHalSettings(SPIFrequency))
	{

	}

	void SPIDisplay::setup() {
		Display::setup();

		// Resetting CS pin just in case
		_hal->GPIO->setPinOutput(_csPin);
		setChipSelect(true);

		// Initialize non-SPI GPIOs
		_hal->GPIO->setPinOutput(_dcPin);

		// Toggle reset pin if it was defined
		if (_rstPin >= 0) {
			_hal->GPIO->setPinOutput(_rstPin);

			_hal->GPIO->write(_rstPin, 0);
			_hal->sleep(100);

			_hal->GPIO->write(_rstPin, 1);
			_hal->sleep(100);
		}

		// SPI
		_hal->SPI->setup();

		writeSetupCommands();
	}

	void SPIDisplay::writeData(uint8_t data) {
		setChipSelect(false);

		_hal->SPI->writeByte(_SPIHalSettings, data);

		setChipSelect(true);
	}

	void SPIDisplay::writeData(const uint8_t* data, size_t length) {
		setChipSelect(false);

		_hal->SPI->writeBytes(_SPIHalSettings, data, length);

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
		_hal->GPIO->write(_csPin, value);
	}

	void SPIDisplay::setDataCommand(uint8_t value) const {
		_hal->GPIO->write(_dcPin, value);
	}

	MCUHal* SPIDisplay::getHal() {
		return _hal;
	}
}