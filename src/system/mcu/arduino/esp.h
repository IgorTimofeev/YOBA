#pragma once

#include <cstdint>
#include <functional>

namespace yoba::system {
	void sleep(uint32_t milliseconds);

	uint32_t getTime();

	namespace gpio {
		void setInput(uint8_t pin);
		void setOutput(uint8_t pin);
		bool read(uint8_t pin);
		void write(uint8_t pin, bool value);
		void setOnInterrupt(uint8_t pin, std::function<void()> callback);
	}

	class spi {
		public:
			static void setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency);
			static void writeByte(uint8_t data);
			static void writeBytes(const uint8_t* data, size_t length);
			static uint32_t _frequency;
	};

	namespace i2c {
		void setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency);
		uint8_t readByte(uint8_t registerAddress);
		void writeByte(uint8_t registerAddress, uint8_t data);
	}
}