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

	uint8_t ScreenDriver::getTransactionBufferHeight() const {
		return _transactionBufferHeight;
	}

	void ScreenDriver::rotatePoint(Point& point) {
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
			.max_transfer_sz = _transactionBufferHeight * _size.getWidth() * 2 + 8
		};

		spi_device_interface_config_t deviceConfig = {
			.mode = 0,                              //SPI mode 0
			.clock_speed_hz = getSPIFrequency(),     //Clock out at required MHz
			.spics_io_num = _csPin,             //CS pin
			.queue_size = 7,                        //We want to be able to queue 7 transactions at a time
			.pre_cb = SPIPreCallback, //Specify pre-transfer callback to handle D/C line
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

	void ScreenDriver::sendCommand(uint8_t command) {
		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction));       //Zero out the transaction
		transaction.length = 8;                   //Command is 8 bits
		transaction.tx_buffer = &command;             //The data is the cmd itself
		transaction.user = new DriverSPIPreCallbackUserData(this, false);              //D/C needs to be set to 0
	//	transaction.flags = SPI_TRANS_CS_KEEP_ACTIVE;   //Keep CS active after data transfer

		auto result = spi_device_polling_transmit(_spi, &transaction); //Transmit!
		assert(result == ESP_OK);          //Should have had no issues.
	}

	void ScreenDriver::sendData(const uint8_t *data, int length) {
		if (length == 0)
			return;    //no need to send anything

		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction));       //Zero out the transaction
		transaction.length = length * 8;             //Len is in bytes, transaction length is in bits.
		transaction.tx_buffer = data;             //Data
		transaction.user = new DriverSPIPreCallbackUserData(this, true); //D/C needs to be set to 1

		auto result = spi_device_polling_transmit(_spi, &transaction); //Transmit!
		assert(result == ESP_OK);          //Should have had no issues.
	}

	void ScreenDriver::SPIPreCallback(spi_transaction_t *transaction) {
		auto userData = (DriverSPIPreCallbackUserData*) transaction->user;
		gpio_set_level((gpio_num_t) 16, userData->dcPinState);
		delete userData;
	}

	void ScreenDriver::flushTransactionBuffer(int y) {
		//Transaction descriptors. Declared static, so they're not allocated on the stack; we need this memory even when this
		//function is finished because the SPI driver needs access to it even while we're already calculating the next line.
		static spi_transaction_t transactions[6];

		// In theory, it's better to initialize trans and data only once and hang on to the initialized
		// variables. We allocate them on the stack, so we need to re-init them each call.
		for (uint8_t i = 0; i < 6; i++) {
			memset(&transactions[i], 0, sizeof(spi_transaction_t));

			if ((i & 1) == 0) {
				//Even transfers are commands
				transactions[i].length = 8;
				transactions[i].user = new DriverSPIPreCallbackUserData(this, false);
			}
			else {
				// Odd transfers are data
				transactions[i].length = 8 * 4;
				transactions[i].user = new DriverSPIPreCallbackUserData(this, true);
			}

			transactions[i].flags = SPI_TRANS_USE_TXDATA;
		}

		transactions[0].tx_data[0] = 0x2A;         //Column Address Set

		transactions[1].tx_data[0] = 0;            //Start Col High
		transactions[1].tx_data[1] = 0;            //Start Col Low
		transactions[1].tx_data[2] = (_size.getWidth() - 1) >> 8;   //End Col High
		transactions[1].tx_data[3] = (_size.getWidth() - 1) & 0xff; //End Col Low

		transactions[2].tx_data[0] = 0x2B;         //Page address set

		transactions[3].tx_data[0] = y >> 8;    //Start page high
		transactions[3].tx_data[1] = y & 0xff;  //start page low
		transactions[3].tx_data[2] = (y + _transactionBufferHeight - 1) >> 8; //end page high
		transactions[3].tx_data[3] = (y + _transactionBufferHeight - 1) & 0xff; //end page low

		transactions[4].tx_data[0] = 0x2C;         //memory write

		transactions[5].tx_buffer = _transactionBuffer;      //finally send the line data
		transactions[5].length = _size.getWidth() * _transactionBufferHeight * 2 * 8;  //Data length, in bits
		transactions[5].flags = 0; //undo SPI_TRANS_USE_TXDATA flag

		// Enqueue all transactions
		esp_err_t result;

		for (uint8_t i = 0; i < 6; i++) {
			result = spi_device_queue_trans(_spi, &transactions[i], portMAX_DELAY);
			assert(result == ESP_OK);
		}

		//When we are here, the SPI driver is busy (in the background) getting the transactions sent. That happens
		//mostly using DMA, so the CPU doesn't have much to do here. We're not going to wait for the transaction to
		//finish because we may as well spend the time calculating the next line. When that is done, we can call
		//send_line_finish, which will wait for the transfers to be done and check their status.

		spi_transaction_t* transaction;

		//Wait for all 6 transactions to be done and get back the results.
		for (uint8_t i = 0; i < 6; i++) {
			result = spi_device_get_trans_result(_spi, &transaction, portMAX_DELAY);
			assert(result == ESP_OK);

			//We could inspect rtrans now if we received any info back. The LCD is treated as write-only, though.
		}
	}

	uint16_t *ScreenDriver::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t ScreenDriver::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	void ScreenDriver::sendCommandAndData(uint8_t command, const uint8_t *data, int length) {
		sendCommand(command);
		sendData(data, length);
	}

	void ScreenDriver::sendCommandAndData(uint8_t command, uint8_t data) {
		uint8_t buffer[] {
			data
		};

		sendCommandAndData(command, buffer, 1);
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
		_transactionBufferHeight = getTransactionBufferHeightForOrientation();

		// Allocating transaction buffer
		delete _transactionBuffer;
		_transactionBufferLength = _size.getWidth() * _transactionBufferHeight * 2;
		_transactionBuffer = (uint16_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
		assert(_transactionBuffer != nullptr);
	}
}