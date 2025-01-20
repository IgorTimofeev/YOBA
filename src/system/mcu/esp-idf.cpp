#include "../../system.h"

#ifdef YOBA_SYSTEM_ESPIDF

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FunctionalInterrupt.h"

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
				.pull_down_en = GPIO_PULLDOWN_DISABLE
			};

			gpio_config(&conf);
		}

		void setOutput(uint8_t pin) {
			gpio_config_t conf = {
				.pin_bit_mask = 1ULL << pin,
				.mode = GPIO_MODE_OUTPUT,
				.pull_up_en = GPIO_PULLUP_DISABLE,
				.pull_down_en = GPIO_PULLDOWN_DISABLE
			};

			gpio_config(&conf);
		}

		bool read(uint8_t pin) {
			return gpio_get_level((gpio_num_t) pin);
		}

		void write(uint8_t pin, bool value) {
			gpio_set_level((gpio_num_t) pin, value);
		}

		void setOnInterrupt(uint8_t pin, std::function<void()> callback) {
			gpio_install_isr_service((int) ESP_INTR_FLAG_IRAM);
			gpio_set_intr_type((gpio_num_t) pin, GPIO_INTR_ANYEDGE);

			// this uses function typecasting, which is not defined when the functions have different signatures
			// untested and might not work
			gpio_isr_handler_add((gpio_num_t)pin, (void (*)(void*)) callback, NULL);
		}
	}

	namespace spi {
		void setup() {
			SPI.begin();
		}

		void writeByte(uint32_t frequency, uint8_t data) {
			SPI.beginTransaction(SPISettings(frequency, SPI_MSBFIRST, SPI_MODE0));
			SPI.write(data);
			SPI.endTransaction();
		}

		void writeBytes(uint32_t frequency, const uint8_t* data, size_t length) {
			SPI.beginTransaction(SPISettings(frequency, SPI_MSBFIRST, SPI_MODE0));
			SPI.writeBytes(data, length);
			SPI.endTransaction();
		}
	}

	namespace i2c {
		void setup(uint8_t sdaPin, uint8_t sclPin, uint32_t frequency) {
			Wire.begin(sdaPin, sclPin, frequency);
		}

		uint8_t readByte(uint16_t slaveAddress, uint8_t registerAddress) {
			uint8_t result = 0;
			uint8_t readLength;

			do {
				Wire.beginTransmission(slaveAddress);
				Wire.write(registerAddress);
				Wire.endTransmission(false);
				// Restart
				// system::sleep(10);
				readLength = Wire.requestFrom(slaveAddress, (uint8_t) 1);
			}
			while (readLength == 0);

			while (Wire.available()) {
				result = Wire.read();
			}

			return result;
		}

		void writeByte(uint16_t slaveAddress, uint8_t registerAddress, uint8_t data) {
			Wire.beginTransmission(slaveAddress);
			Wire.write(registerAddress);
			Wire.write(data);
			Wire.endTransmission();
		}
	}
}

#endif