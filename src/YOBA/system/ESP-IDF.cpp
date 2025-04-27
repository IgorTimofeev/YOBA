#include "YOBA/system.h"

#ifdef YOBA_SYSTEM_ESPIDF

#include <cstdio>
#include <cstring>
#include <esp_timer.h>
#include <unordered_map>
#include <functional>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include "driver/i2c_master.h"

namespace YOBA::system {
	// -------------------------------- System --------------------------------

	void sleep(uint32_t milliseconds) {
		vTaskDelay(milliseconds / portTICK_PERIOD_MS);
	}

	uint32_t getTime() {
		return (uint32_t) (esp_timer_get_time() / 1000ULL);
	}

	std::unordered_map<uint8_t, std::function<void()>> GPIO::_interruptHandlers = {};

	// -------------------------------- GPIO --------------------------------

	void GPIO::setMode(uint8_t pin, PinMode mode) {
		gpio_config_t config = {
			.pin_bit_mask = 1ULL << pin,
			.mode = mode == PinMode::Input ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_ENABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
		};

		gpio_config(&config);
	}

	bool GPIO::read(uint8_t pin) {
		return gpio_get_level((gpio_num_t) pin);
	}

	void GPIO::write(uint8_t pin, bool value) {
		gpio_set_level((gpio_num_t) pin, value);
	}

	void GPIO::interruptHandler(void *args) {
		auto funcPtr = (std::function<void()>*) args;

		(*funcPtr)();
	}

	void GPIO::addInterruptHandler(uint8_t pin, const std::function<void()>& callback) {
		_interruptHandlers.insert({pin, callback});

		gpio_install_isr_service(0);
		gpio_set_intr_type((gpio_num_t) pin, GPIO_INTR_ANYEDGE);
		gpio_isr_handler_add((gpio_num_t) pin, interruptHandler, (void*) &_interruptHandlers[pin]);
	}

	// -------------------------------- SPI --------------------------------

	spi_device_handle_t SPI::_deviceHandle = spi_device_handle_t();

	// Note: SPI instance will manage slave select output by itself
	void SPI::setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency) {
		// GPIO
		GPIO::setMode(ssPin, system::GPIO::PinMode::Output);
		GPIO::write(ssPin, true);

		// Bus
		spi_bus_config_t busConfig {};
		busConfig.mosi_io_num = mosiPin;
		busConfig.miso_io_num = -1;
		busConfig.sclk_io_num = sckPin;
		busConfig.quadwp_io_num = -1;
		busConfig.quadhd_io_num = -1;
		busConfig.max_transfer_sz = 320 * 240 * 2;

		// May be already initialized
		const auto result = spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO);
		assert(result == ESP_OK || result == ESP_ERR_INVALID_STATE);

		// Interface
		spi_device_interface_config_t interfaceConfig {};
		interfaceConfig.mode = 0;
		interfaceConfig.clock_speed_hz = (int) frequency;
		interfaceConfig.spics_io_num = ssPin;
		interfaceConfig.flags = SPI_DEVICE_NO_DUMMY;
		interfaceConfig.queue_size = 1;

		ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_deviceHandle));
	}

	void SPI::write(uint8_t data) {
		spi_transaction_t transaction {};
		transaction.tx_data[0] = data;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.length = 8;

		ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &transaction));
	}

	void SPI::write(const uint8_t* data, size_t length) {
		spi_transaction_t transaction {};
		transaction.length = length * 8;
		transaction.tx_buffer = data;

		ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &transaction));
	}

	// -------------------------------- I2C --------------------------------

	i2c_master_bus_handle_t I2C::_busHandle = {};
	i2c_master_dev_handle_t I2C::_deviceHandle = {};

	void I2C::setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency) {
		i2c_master_bus_config_t busConfig {};
		busConfig.i2c_port = I2C_NUM_0;
		busConfig.sda_io_num = (gpio_num_t) sdaPin;
		busConfig.scl_io_num = (gpio_num_t) sclPin;
		busConfig.clk_source = I2C_CLK_SRC_DEFAULT;
		busConfig.glitch_ignore_cnt = 7;
		busConfig.flags.enable_internal_pullup = true;
		busConfig.flags.allow_pd = false;

		// May be already initialized
		i2c_new_master_bus(&busConfig, &_busHandle);

		i2c_device_config_t deviceConfig {};
		deviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
		deviceConfig.device_address = slaveAddress;
		deviceConfig.scl_speed_hz = frequency;

		ESP_ERROR_CHECK(i2c_master_bus_add_device(_busHandle, &deviceConfig, &_deviceHandle));
	}

	void I2C::read(uint8_t* buffer, size_t length) {
		ESP_ERROR_CHECK(i2c_master_receive(_deviceHandle, buffer, length, -1));
	}

	void I2C::write(uint8_t* buffer, size_t length) {
		ESP_ERROR_CHECK(i2c_master_transmit(_deviceHandle, buffer, length, -1));
	}
}

#endif