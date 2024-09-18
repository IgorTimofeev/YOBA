#include <SPI.h>
#include "screenDriver.h"
#include "Arduino.h"
#include "point.h"

namespace yoba {
	ScreenDriver::ScreenDriver(uint8_t csPin, uint8_t dcPin, int8_t rstPin, uint32_t spiFrequency, const Size& defaultSize, ScreenOrientation orientation) :
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),

		_spiSettings(SPISettings(spiFrequency, SPI_MSBFIRST, SPI_MODE0)),
		_defaultSize(defaultSize),
		_size(defaultSize),
		_orientation(orientation)
	{

	}

	const Size& ScreenDriver::getSize() const {
		return _size;
	}

	ScreenOrientation ScreenDriver::getOrientation() const {
		return _orientation;
	}

	void ScreenDriver::setOrientation(ScreenOrientation orientation) {
		_orientation = orientation;

		updateDataFromOrientation();
		writeOrientationChangeCommands();
	}

	uint8_t ScreenDriver::getTransactionWindowHeight() const {
		return _transactionWindowHeight;
	}

	void ScreenDriver::rotatePointForOrientation(Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case ScreenOrientation::Portrait0:
				break;

			case ScreenOrientation::Landscape90: {
				int32_t x = point.getX();
				point.setX(point.getY());
				point.setY(_size.getHeight() - x);

				break;
			}

			case ScreenOrientation::Portrait180:
				point.setX(_size.getWidth() - point.getX());
				point.setY(_size.getHeight() - point.getY());

				break;

			case ScreenOrientation::Landscape270: {
				int32_t x = point.getX();
				point.setX(_size.getWidth() - point.getY());
				point.setY(x);

				break;
			}
		}
	}

	void ScreenDriver::begin() {
		// Resetting CS pin just in case
		pinMode(_csPin, OUTPUT);
		setChipSelect(true);

		SPI.begin();

		updateDataFromOrientation();

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

		writeBeginCommands();
	}

	inline void ScreenDriver::writeData(uint8_t data) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transfer(data);
		SPI.endTransaction();

		setChipSelect(true);
	}

	inline void ScreenDriver::writeData(const uint8_t* data, int length) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transferBytes(data, nullptr, length);
		SPI.endTransaction();

		setChipSelect(true);
	}

	void ScreenDriver::writeCommand(uint8_t data) {
		setDataCommand(false);
		writeData(data);
		setDataCommand(true);
	}

	void ScreenDriver::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	void ScreenDriver::writeCommandAndData(uint8_t command, const uint8_t *data, int length) {
		writeCommand(command);
		writeData(data, length);
	}

	void ScreenDriver::flushTransactionBuffer(int y) {
		uint8_t data[4];

		// Column Address Set
		data[0] = 0; //Start Col High
		data[1] = 0; //Start Col Low
		data[2] = (_size.getWidth() - 1) >> 8; //End Col High
		data[3] = (_size.getWidth() - 1) & 0xff; //End Col Low
		writeCommandAndData(0x2A, data, 4);

		//Page address set
		data[0] = y >> 8; //Start page high
		data[1] = y & 0xff; // Start page low
		data[2] = (y + _transactionWindowHeight - 1) >> 8; // End page high
		data[3] = (y + _transactionWindowHeight - 1) & 0xff; // End page low
		writeCommandAndData(0x2B, data, 4);

		// Memory write
		writeCommandAndData(0x2C, (uint8_t*) _transactionBuffer, _size.getWidth() * _transactionWindowHeight * 2);
	}

	uint16_t *ScreenDriver::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t ScreenDriver::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	void ScreenDriver::updateDataFromOrientation() {
		// Updating size
		switch (_orientation) {
			case ScreenOrientation::Portrait0:
				_size.setWidth(_defaultSize.getWidth());
				_size.setHeight(_defaultSize.getHeight());
				break;

			case ScreenOrientation::Landscape90:
				_size.setWidth(_defaultSize.getHeight());
				_size.setHeight(_defaultSize.getWidth());
				break;

			case ScreenOrientation::Portrait180:
				_size.setWidth(_defaultSize.getWidth());
				_size.setHeight(_defaultSize.getHeight());
				break;

			case ScreenOrientation::Landscape270:
				_size.setWidth(_defaultSize.getHeight());
				_size.setHeight(_defaultSize.getWidth());
				break;
		}

		// Updating transaction buffer height
		_transactionWindowHeight = getTransactionWindowHeightForOrientation();

		// Allocating transaction buffer
		delete _transactionBuffer;
		_transactionBufferLength = _size.getWidth() * _transactionWindowHeight * 2;
		_transactionBuffer = (uint16_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
		assert(_transactionBuffer != nullptr);
	}

	void ScreenDriver::setChipSelect(uint8_t value) const {
		digitalWrite(_csPin, value);
	}

	void ScreenDriver::setDataCommand(uint8_t value) const {
		digitalWrite(_dcPin, value);
	}
}