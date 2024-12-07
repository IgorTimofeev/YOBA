#include "transactionalSPIScreenDriver.h"

namespace yoba {
	TransactionalSPIScreenDriver::TransactionalSPIScreenDriver(
		ScreenDriverPixelAlignment pixelAlignment,
		ColorModel colorModel,
		const Size& resolution,
		ScreenOrientation orientation,
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t spiFrequency
	) : SPIScreenDriver(ScreenDriverPixelWritingType::Transactional, pixelAlignment, colorModel, resolution, orientation, csPin, dcPin, rstPin, spiFrequency) {

	}

	uint8_t TransactionalSPIScreenDriver::getTransactionWindowHeight() const {
		return _transactionWindowHeight;
	}

	void TransactionalSPIScreenDriver::writeTransactionBuffer(uint16_t y) {
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

	void TransactionalSPIScreenDriver::writePixels(const std::function<void(uint8_t*&, size_t&)>& pixelSetter) {
		size_t pixelIndex = 0;
		uint16_t y;
		uint8_t* transactionBufferPtr;
		uint8_t* transactionBufferEnd = _transactionBuffer + _transactionBufferLength;

		for (y = 0; y < this->_resolution.getHeight(); y += _transactionWindowHeight) {
			transactionBufferPtr = _transactionBuffer;

			while (transactionBufferPtr < transactionBufferEnd) {
				pixelSetter(transactionBufferPtr, pixelIndex);
			}

			writeTransactionBuffer(y);
		}
	}

	uint8_t* TransactionalSPIScreenDriver::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t TransactionalSPIScreenDriver::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	void TransactionalSPIScreenDriver::updateDataFromOrientation() {
		ScreenDriver::updateDataFromOrientation();

		// Updating transaction buffer height
		_transactionWindowHeight = getTransactionWindowHeightForOrientation();

		// Allocating transaction buffer
		delete _transactionBuffer;
		_transactionBufferLength = Color::getBytesPerType(this->_resolution.getWidth() * _transactionWindowHeight, _colorModel);
		_transactionBuffer = (uint8_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
		assert(_transactionBuffer != nullptr);
	}
}