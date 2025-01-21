#include "../../system.h"

#ifdef YOBA_SYSTEM_ESPIDF

#include <cstdio>
#include <cstring>
#include <esp_timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include "driver/i2c_master.h"

namespace yoba::system {
	void sleep(uint32_t milliseconds) {
		vTaskDelay(milliseconds / portTICK_PERIOD_MS);
	}

	uint32_t getTime() {
		return (uint32_t) (esp_timer_get_time() / 1000ULL);
	}

	namespace gpio {
		void setInput(uint8_t pin) {
			gpio_config_t conf = {
				.pin_bit_mask = 1ULL << pin,
				.mode = GPIO_MODE_INPUT,
				.pull_up_en = GPIO_PULLUP_DISABLE,
				.pull_down_en = GPIO_PULLDOWN_DISABLE,
				.intr_type = GPIO_INTR_DISABLE
			};

			gpio_config(&conf);
		}

		void setOutput(uint8_t pin) {
			gpio_config_t conf = {
				.pin_bit_mask = 1ULL << pin,
				.mode = GPIO_MODE_OUTPUT,
				.pull_up_en = GPIO_PULLUP_DISABLE,
				.pull_down_en = GPIO_PULLDOWN_DISABLE,
				.intr_type = GPIO_INTR_DISABLE
			};

			gpio_config(&conf);
		}

		bool read(uint8_t pin) {
			return gpio_get_level((gpio_num_t) pin);
		}

		void write(uint8_t pin, bool value) {
			gpio_set_level((gpio_num_t) pin, value);
		}

		static void IRAM_ATTR gpio_interrupt_handler(void *args) {
			(*((std::function<void()>*) args))();
		}

		void setOnInterrupt(uint8_t pin, std::function<void()> callback) {
			gpio_install_isr_service((int) ESP_INTR_FLAG_IRAM);
			gpio_set_intr_type((gpio_num_t) pin, GPIO_INTR_ANYEDGE);
			gpio_isr_handler_add((gpio_num_t) pin, gpio_interrupt_handler, (void*) &callback);
		}
	}

	namespace spi {
		// TODO: refactor this
		spi_device_handle_t _spi;
		uint32_t _frequency;
		uint8_t _ssPin;

		void setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency) {
			_ssPin = ssPin;
			_frequency = frequency;

			esp_err_t ret;

			spi_bus_config_t busConfig = {
				.mosi_io_num = mosiPin,
				.miso_io_num = -1,
				.sclk_io_num = sckPin,
				.quadwp_io_num = -1,
				.quadhd_io_num = -1,
				.max_transfer_sz = 240 * 64 * 8 * 2,
			};

			spi_device_interface_config_t interfaceConfig = {
				.mode = 0,                              //SPI mode 0
				.clock_speed_hz = (int) frequency,     //Clock out at 26 MHz
				.spics_io_num = -1,             //CS pin
				.queue_size = 1,                        //We want to be able to queue 7 transactions at a time
			};

			//Initialize the SPI bus
			ret = spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO);
			ESP_ERROR_CHECK(ret);

			//Attach the LCD to the SPI bus
			ret = spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_spi);
			ESP_ERROR_CHECK(ret);
		}

		void writeByte(uint8_t data) {
			esp_err_t ret;
			spi_transaction_t transaction;
			memset(&transaction, 0, sizeof(transaction));       //Zero out the transaction
			transaction.length = 8;                   //Command is 8 bits
			transaction.tx_buffer = &data;             //The data is the cmd itself
			ret = spi_device_transmit(_spi, &transaction); //Transmit!
			assert(ret == ESP_OK);          //Should have had no issues.
		}

		void writeBytes(const uint8_t* data, size_t length) {
			esp_err_t ret;
			spi_transaction_t t;
			if (length == 0) {
				return;    //no need to send anything
			}
			memset(&t, 0, sizeof(t));       //Zero out the transaction
			t.length = length * 8;             //Len is in bytes, transaction length is in bits.
			t.tx_buffer = data;             //Data
			ret = spi_device_transmit(_spi, &t); //Transmit!
			assert(ret == ESP_OK);          //Should have had no issues.
		}
	}

	namespace i2c {
		i2c_master_dev_handle_t _deviceHandle;

		void setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency) {
			i2c_master_bus_config_t busConfig = {
				.i2c_port = I2C_NUM_0,
				.sda_io_num = (gpio_num_t) sdaPin,
				.scl_io_num = (gpio_num_t) sclPin,
				.clk_source = I2C_CLK_SRC_DEFAULT,
				.glitch_ignore_cnt = 7,
			};

			i2c_master_bus_handle_t busHandle;

			ESP_ERROR_CHECK(i2c_new_master_bus(&busConfig, &busHandle));

			i2c_device_config_t deviceConfig = {
				.dev_addr_length = I2C_ADDR_BIT_LEN_7,
				.device_address = slaveAddress,
				.scl_speed_hz = frequency,
			};

			ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &deviceConfig, &_deviceHandle));
		}

		uint8_t readByte(uint8_t registerAddress) {
			uint8_t result = 0;

			ESP_ERROR_CHECK(i2c_master_transmit(_deviceHandle, &registerAddress, 1, -1));
			ESP_ERROR_CHECK(i2c_master_receive(_deviceHandle, &result, 1, -1));

			return result;
		}

		void writeByte(uint8_t registerAddress, uint8_t data) {
			uint8_t govno[2] = {
				registerAddress,
				data
			};

			ESP_ERROR_CHECK(i2c_master_transmit(_deviceHandle, govno, 2, -1));
		}
	}
}

#endif