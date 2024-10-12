#include "Arduino.h"
#include <SPI.h>
#include "SPIScreenDriver.h"

namespace yoba {
	SPIScreenDriver::SPIScreenDriver(
		ColorModel colorModel,
		const Size& resolution,
		ScreenOrientation orientation,

		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		ScreenDriver(
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
			vTaskDelay(100 / portTICK_PERIOD_MS);

			digitalWrite(_rstPin, 1);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		// SPI
		SPI.begin();

		writeBeginCommands();
	}

	uint8_t SPIScreenDriver::getTransactionWindowHeight() const {
		return _transactionWindowHeight;
	}

	inline void SPIScreenDriver::writeData(uint8_t data) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transfer(data);
		SPI.endTransaction();

		setChipSelect(true);
	}

	inline void SPIScreenDriver::writeData(const uint8_t* data, size_t length) {
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

	void SPIScreenDriver::flushTransactionBuffer(uint16_t y) {
		uint8_t data[4];

		// Column Address Set
		data[0] = 0; //Start Col High
		data[1] = 0; //Start Col Low
		data[2] = (this->_resolution.getWidth() - 1) >> 8; //End Col High
		data[3] = (this->_resolution.getWidth() - 1) & 0xff; //End Col Low
		writeCommandAndData(0x2A, data, 4);

		//Page address set
		data[0] = y >> 8; //Start page high
		data[1] = y & 0xff; // Start page low
		data[2] = (y + _transactionWindowHeight - 1) >> 8; // End page high
		data[3] = (y + _transactionWindowHeight - 1) & 0xff; // End page low
		writeCommandAndData(0x2B, data, 4);

		// Memory write
		writeCommandAndData(0x2C, _transactionBuffer, _transactionBufferLength);
	}

	uint8_t* SPIScreenDriver::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t SPIScreenDriver::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	void SPIScreenDriver::updateDataFromOrientation() {
		ScreenDriver::updateDataFromOrientation();

		// Updating transaction buffer height
		_transactionWindowHeight = getTransactionWindowHeightForOrientation();

		// Allocating transaction buffer
		delete _transactionBuffer;
		_transactionBufferLength = Color::getBytesForPixelsPerType(this->_resolution.getWidth() * _transactionWindowHeight, _colorModel);
		_transactionBuffer = (uint8_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
		assert(_transactionBuffer != nullptr);
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

	void SPIScreenDriver::writePixels(const std::function<uint32_t(size_t pixelIndex)>& pixelGetter) {
		switch (_colorModel) {
			case ColorModel::Rgb565: {
				const size_t pixelCount = this->_resolution.getWidth() * _transactionWindowHeight;
				size_t pixelIndex = 0;

				for (uint16_t y = 0; y < this->_resolution.getHeight(); y += _transactionWindowHeight) {
					for (size_t transactionBufferIndex = 0; transactionBufferIndex < pixelCount; transactionBufferIndex++) {
						((uint16_t*) _transactionBuffer)[transactionBufferIndex] = pixelGetter(pixelIndex);
						pixelIndex++;
					}

					flushTransactionBuffer(y);
				}
				break;
			}

			default:
				break;
		}
	}
}