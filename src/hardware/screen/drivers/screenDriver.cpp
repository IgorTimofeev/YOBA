#include "screenDriver.h"
#include "Arduino.h"
#include "point.h"

namespace yoba {
	ScreenDriver::ScreenDriver(uint8_t csPin, uint8_t dcPin, int8_t rstPin, const Size& defaultSize, ScreenOrientation orientation) :
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),

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

	int32_t ScreenDriver::getSPIFrequency() const {
		return _spiFrequency;
	}

	void ScreenDriver::setSPIFrequency(int32_t spiFrequency) {
		_spiFrequency = spiFrequency;
	}

	void ScreenDriver::writeInitializationCommands() {

	}

	void ScreenDriver::begin() {
		updateDataFromOrientation();

		spi_bus_config_t busConfig = {
			.mosi_io_num = MOSI,
			.miso_io_num = MISO,
			.sclk_io_num = SCK,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1,
			.max_transfer_sz = _transactionWindowHeight * _size.getWidth() * 2 + 8
		};

		spi_device_interface_config_t deviceConfig = {
			.mode = 0,                              //SPI mode 0
			.clock_speed_hz = getSPIFrequency(),     //Clock out at required MHz
			.spics_io_num = _csPin,             //CS pin
			.queue_size = 7,                        //We want to be able to queue 7 transactions at a time
			.pre_cb = SpiPreTransferCallback, //Specify pre-transfer callback to handle D/C line
		};

		//Initialize the SPI bus
		auto SPIHost = SPI2_HOST;

		esp_err_t result = spi_bus_initialize(SPIHost, &busConfig, SPI_DMA_CH_AUTO);
		ESP_ERROR_CHECK(result);

		//Attach the LCD to the SPI bus
		result = spi_bus_add_device(SPIHost, &deviceConfig, &_spi);
		ESP_ERROR_CHECK(result);

		//Initialize non-SPI GPIOs
		uint64_t gpioConfigPinMask = 1ULL << _dcPin;

		if (_rstPin >= 0)
			gpioConfigPinMask |= 1ULL << _rstPin;

		gpio_config_t gpioConfig {
			.pin_bit_mask = gpioConfigPinMask,
			.mode = GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_ENABLE
		};

		gpio_config(&gpioConfig);

		// Toggle reset pin if it was defined
		if (_rstPin >= 0) {
			gpio_set_level((gpio_num_t) _rstPin, 0);
			vTaskDelay(100 / portTICK_PERIOD_MS);

			gpio_set_level((gpio_num_t) _rstPin, 1);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		writeInitializationCommands();
	}

	void ScreenDriver::writeData(uint8_t data, bool dcPinState) {
		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction));       //Zero out the transaction
		transaction.length = 8;                   //Command is 8 bits
		transaction.tx_data[0] = data;             //The data is the cmd itself
		transaction.flags = SPI_TRANS_USE_TXDATA;
		//	transaction.flags = SPI_TRANS_CS_KEEP_ACTIVE;   //Keep CS active after data transfer

		// D/C needs to be set to 0
		auto userData = DriverSPIPreCallbackUserData(this, dcPinState);
		transaction.user = &userData;

		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);
	}

	void ScreenDriver::writeData(const uint8_t *data, int length, bool dcPinState) {
		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction)); //Zero out the transaction
		transaction.length = length * 8; // Length is in bytes, transaction length is in bits.
		transaction.tx_buffer = data; //Data

		// D/C needs to be set to 1
		auto userData = DriverSPIPreCallbackUserData(this, dcPinState);
		transaction.user = &userData;

		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);
	}

	void ScreenDriver::writeCommand(uint8_t data) {
		writeData(data, false);
	}

	void ScreenDriver::writeCommandAndData(uint8_t command, const uint8_t *data, int length) {
		writeCommand(command);
		writeData(data, length, true);
	}

	void ScreenDriver::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data, true);
	}

	void ScreenDriver::SpiPreTransferCallback(spi_transaction_t *transaction) {
		auto userData = (DriverSPIPreCallbackUserData*) transaction->user;
		gpio_set_level((gpio_num_t) userData->driver->_dcPin, userData->dcPinState);
	}

	void ScreenDriver::flushTransactionBuffer(int y) {
		static spi_transaction_t transaction;
		auto falseUserData = DriverSPIPreCallbackUserData(this, false);
		auto trueUserData = DriverSPIPreCallbackUserData(this, true);

		// Column Address Set
		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.length = 8;
		transaction.tx_data[0] = 0x2A;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.user = &falseUserData;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);

		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.length = 8 * 4;
		transaction.tx_data[0] = 0; //Start Col High
		transaction.tx_data[1] = 0; //Start Col Low
		transaction.tx_data[2] = (_size.getWidth() - 1) >> 8; //End Col High
		transaction.tx_data[3] = (_size.getWidth() - 1) & 0xff; //End Col Low
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.user = &trueUserData;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);

		//Page address set
		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.length = 8;
		transaction.tx_data[0] = 0x2B;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.user = &falseUserData;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);

		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.length = 8 * 4;
		transaction.tx_data[0] = y >> 8; //Start page high
		transaction.tx_data[1] = y & 0xff; // Start page low
		transaction.tx_data[2] = (y + _transactionWindowHeight - 1) >> 8; // End page high
		transaction.tx_data[3] = (y + _transactionWindowHeight - 1) & 0xff; // End page low
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.user = &trueUserData;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);

		// Memory write
		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.length = 8;
		transaction.tx_data[0] = 0x2C;
		transaction.user = &falseUserData;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);

		// Sending transaction buffer
		memset(&transaction, 0, sizeof(spi_transaction_t));
		transaction.tx_buffer = _transactionBuffer;
		transaction.length = _size.getWidth() * _transactionWindowHeight * 2 * 8;  // Data length, in bits
		transaction.flags = 0; // Undo SPI_TRANS_USE_TXDATA flag
		transaction.user = &trueUserData;
		assert(spi_device_polling_transmit(_spi, &transaction) == ESP_OK);
	}

	uint16_t *ScreenDriver::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t ScreenDriver::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	DriverSPIPreCallbackUserData::DriverSPIPreCallbackUserData(ScreenDriver *driver, bool dcPinState) : driver(driver), dcPinState(dcPinState) {}

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
}