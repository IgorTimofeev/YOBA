#pragma once

#include <cstdint>
#include <functional>

namespace yoba::hardware {
	class SystemHal {
		public:
			virtual void sleep(uint32_t milliseconds) = 0;
			virtual uint32_t getTime() = 0;


	};

	class GPIOHal {
		public:
			virtual void setPinInput(uint8_t pin) = 0;
			virtual void setPinOutput(uint8_t pin) = 0;
			virtual bool read(uint8_t pin) = 0;
			virtual void write(uint8_t pin, bool value) = 0;
			virtual void setOnInterrupt(uint8_t pin, std::function<void()> callback) = 0;
	};

	class SPIHalSettings {
		public:
			SPIHalSettings(uint32_t frequency) : _frequency(frequency) {

			}

			uint32_t getFrequency() const {
				return _frequency;
			}

		private:
			uint32_t _frequency;
	};

	class SPIHal {
		public:
			virtual void setup() = 0;

			virtual void writeByte(const SPIHalSettings& settings, uint8_t data) = 0;
			virtual void writeBytes(const SPIHalSettings& settings, const uint8_t* data, size_t length) = 0;
	};

	class I2CHal {
		public:
			virtual void setup(uint8_t sdaPin, uint8_t sclPin, uint32_t frequency = 0) = 0;
			virtual uint8_t readByte(uint16_t slaveAddress, uint8_t registerAddress) = 0;
			virtual void writeByte(uint16_t slaveAddress, uint8_t registerAddress, uint8_t data) = 0;
	};

	class MCUHal : public SystemHal {
		public:
			MCUHal(GPIOHal* gpio, SPIHal* spi, I2CHal* i2c) : GPIO(gpio), SPI(spi), I2C(i2c) {

			}

			GPIOHal* GPIO;
			SPIHal* SPI;
			I2CHal* I2C;
	};
}