#include "driver.h"
#include "Arduino.h"
#include "yoba/hardware/screen/buffer.h"

Driver::Driver(uint8_t chipSelectPin, uint8_t dataCommandPin, int8_t resetPin) :
	_chipSelectPin(chipSelectPin),
	_dataCommandPin(dataCommandPin),
	_resetPin(resetPin)
{

}

uint8_t Driver::getChipSelectPin() const {
	return _chipSelectPin;
}

void Driver::setChipSelectPin(uint8_t chipSelectPin) {
	_chipSelectPin = chipSelectPin;
}

uint8_t Driver::getDataCommandPin() const {
	return _dataCommandPin;
}

void Driver::setDataCommandPin(uint8_t dataCommandPin) {
	_dataCommandPin = dataCommandPin;
}

int8_t Driver::getResetPin() const {
	return _resetPin;
}

void Driver::setResetPin(int8_t resetPin) {
	_resetPin = resetPin;
}

int32_t Driver::getSPIFrequency() const {
	return _SPIFrequency;
}

void Driver::setSPIFrequency(int32_t spiFrequency) {
	_SPIFrequency = spiFrequency;
}

uint8_t Driver::getTransactionBufferHeight() const {
	return _transactionBufferHeight;
}

void Driver::setTransactionBufferHeight(uint8_t transactionBufferHeight) {
	_transactionBufferHeight = transactionBufferHeight;
}

void Driver::writeInitializationCommands() {

}

void Driver::begin(Buffer* display) {
	spi_bus_config_t busConfig = {
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = SCK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = getTransactionBufferHeight() * display->getSize().getWidth() * 2 + 8
	};

	spi_device_interface_config_t deviceConfig = {
		.mode = 0,                              //SPI mode 0
		.clock_speed_hz = getSPIFrequency(),     //Clock out at required MHz
		.spics_io_num = getChipSelectPin(),             //CS pin
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
	uint64_t gpioConfigPinMask = 1ULL << getDataCommandPin();

	if (getResetPin() >= 0)
		gpioConfigPinMask |= 1ULL << getResetPin();

	gpio_config_t gpioConfig {
		.pin_bit_mask = gpioConfigPinMask,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_ENABLE
	};

	gpio_config(&gpioConfig);

	// Toggle reset pin if it was defined
	if (getResetPin() >= 0) {
		gpio_set_level((gpio_num_t) getResetPin(), 0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		gpio_set_level((gpio_num_t) getResetPin(), 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	writeInitializationCommands();

	//Allocate memory for the transaction buffer
	_transactionBufferLength = display->getSize().getWidth() * getTransactionBufferHeight() * 2;
	_transactionBuffer = (uint16_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
	assert(_transactionBuffer != nullptr);
}

void Driver::sendCommand(uint8_t command) {
	spi_transaction_t transaction;
	memset(&transaction, 0, sizeof(transaction));       //Zero out the transaction
	transaction.length = 8;                   //Command is 8 bits
	transaction.tx_buffer = &command;             //The data is the cmd itself
	transaction.user = new DriverSPIPreCallbackUserData(this, false);              //D/C needs to be set to 0
//	transaction.flags = SPI_TRANS_CS_KEEP_ACTIVE;   //Keep CS active after data transfer

	auto result = spi_device_polling_transmit(_spi, &transaction); //Transmit!
	assert(result == ESP_OK);          //Should have had no issues.
}

void Driver::sendData(const uint8_t *data, int length) {
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

void Driver::SPIPreCallback(spi_transaction_t *transaction) {
	auto userData = (DriverSPIPreCallbackUserData*) transaction->user;
	gpio_set_level((gpio_num_t) 16, userData->dataCommandPinState);
	delete userData;
}

void Driver::flushTransactionBuffer(Buffer* display, int y) {
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
	transactions[1].tx_data[2] = (display->getSize().getWidth() - 1) >> 8;   //End Col High
	transactions[1].tx_data[3] = (display->getSize().getWidth() - 1) & 0xff; //End Col Low

	transactions[2].tx_data[0] = 0x2B;         //Page address set

	transactions[3].tx_data[0] = y >> 8;    //Start page high
	transactions[3].tx_data[1] = y & 0xff;  //start page low
	transactions[3].tx_data[2] = (y + getTransactionBufferHeight() - 1) >> 8; //end page high
	transactions[3].tx_data[3] = (y + getTransactionBufferHeight() - 1) & 0xff; //end page low

	transactions[4].tx_data[0] = 0x2C;         //memory write

	transactions[5].tx_buffer = _transactionBuffer;      //finally send the line data
	transactions[5].length = display->getSize().getWidth() * getTransactionBufferHeight() * 2 * 8;  //Data length, in bits
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

uint16_t *Driver::getTransactionBuffer() const {
	return _transactionBuffer;
}

size_t Driver::getTransactionBufferLength() const {
	return _transactionBufferLength;
}

void Driver::sendCommandAndData(uint8_t command, const uint8_t *data, int length) {
	sendCommand(command);
	sendData(data, length);
}

DriverSPIPreCallbackUserData::DriverSPIPreCallbackUserData(Driver *driver, bool dataCommandPinState) : driver(driver), dataCommandPinState(dataCommandPinState) {}