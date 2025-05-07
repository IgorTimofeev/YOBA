#include "YOBA/system.h"

#ifdef YOBA_SYSTEM_ARDUINO_ESP

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FunctionalInterrupt.h"

namespace YOBA::system {
	void sleep(uint32_t milliseconds) {
		delay(milliseconds);
	}

	uint32_t getTime() {
		return millis();
	}

	namespace gpio {
		void setInput(uint8_t pin) {
			pinMode(pin, INPUT_PULLUP);
		}

		void setOutput(uint8_t pin) {
			pinMode(pin, OUTPUT);
		}

		bool read(uint8_t pin) {
			return digitalRead(pin);
		}

		void write(uint8_t pin, bool value) {
			digitalWrite(pin, value);
		}

		void setOnInterrupt(uint8_t pin, std::function<void()> callback) {
			attachInterrupt(pin, callback, CHANGE);
		}
	}

	uint32_t spi::_frequency = 0;

	void spi::setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency) {
		_frequency = frequency;

		SPI.begin();
	}

	void spi::writeByte(uint8_t data) {
		SPI.beginTransaction(SPISettings(_frequency, SPI_MSBFIRST, SPI_MODE0));
		SPI.write(data);
		SPI.endTransaction();
	}

	void spi::writeBytes(const uint8_t* data, size_t length) {
		SPI.beginTransaction(SPISettings(_frequency, SPI_MSBFIRST, SPI_MODE0));
		SPI.writeBytes(data, length);
		SPI.endTransaction();
	}

	namespace i2c {
		uint16_t _slaveAddress;

		void setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency) {
			_slaveAddress = slaveAddress;

			Wire.begin(sdaPin, sclPin, frequency);
		}

		uint8_t readByte(uint8_t registerAddress) {
			uint8_t result = 0;
			uint8_t readLength;

			do {
				Wire.beginTransmission(_slaveAddress);
				Wire.write(registerAddress);
				Wire.endTransmission(false);
				// Restart
				// system::sleep(10);
				readLength = Wire.requestFrom(_slaveAddress, static_cast<uint8_t>(1));
			}
			while (readLength == 0);

			while (Wire.available()) {
				result = Wire.read();
			}

			return result;
		}

		void writeByte(uint8_t registerAddress, uint8_t data) {
			Wire.beginTransmission(_slaveAddress);
			Wire.write(registerAddress);
			Wire.write(data);
			Wire.endTransmission();
		}
	}
}

#endif