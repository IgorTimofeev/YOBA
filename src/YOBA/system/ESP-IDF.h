#pragma once

#include <cstdint>
#include <functional>
#include <driver/gpio.h>

#include "driver/spi_master.h"
#include "driver/i2c_master.h"

namespace YOBA::system {
	void sleep(uint32_t interval);

	uint64_t getTime();

	class GPIO {
		public:
			enum class PinMode : uint8_t {
				input,
				output
			};

			static void setMode(uint8_t pin, PinMode mode);
			static bool read(uint8_t pin);
			static void write(uint8_t pin, bool value);
			static void addInterruptHandler(uint8_t pin, const gpio_isr_t& callback, void* args);
	};

	class SPI {
		public:
			static void setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency);
			static void write(uint8_t data);
			static void write(const uint8_t* data, size_t length);

			static DRAM_ATTR spi_device_handle_t _deviceHandle;
	};

	class I2C {
		public:
			static void setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency);
			static void read(uint8_t* buffer, size_t length);
			static void write(const uint8_t* buffer, size_t length);

		private:
			static DRAM_ATTR i2c_master_bus_handle_t _busHandle;
			static DRAM_ATTR i2c_master_dev_handle_t _deviceHandle;
	};
}