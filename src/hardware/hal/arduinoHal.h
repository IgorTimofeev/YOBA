#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <FunctionalInterrupt.h>
#include "hal.h"

namespace yoba::hardware {
	class ArduinoGPIOHal : public GPIOHal {
		public:
			void setPinInput(uint8_t pin) override {
				pinMode(pin, INPUT_PULLUP);
			}

			void setPinOutput(uint8_t pin) override {
				pinMode(pin, OUTPUT);
			}

			bool read(uint8_t pin) override {
				return digitalRead(pin);
			}

			void write(uint8_t pin, bool value) override {
				return digitalWrite(pin, value);
			}

			void setOnInterrupt(uint8_t pin, std::function<void()> callback) override {
				attachInterrupt(pin, callback, CHANGE);
			}
	};

	class ArduinoSPIHal : public SPIHal {
		public:
			void setup() override {
				SPI.begin();
			}

			void writeByte(const SPIHalSettings& settings, uint8_t data) override {
				SPI.beginTransaction(SPISettings(settings.getFrequency(), SPI_MSBFIRST, SPI_MODE0));
				SPI.write(data);
				SPI.endTransaction();
			}

			void writeBytes(const SPIHalSettings& settings, const uint8_t* data, size_t length) override {
				SPI.beginTransaction(SPISettings(settings.getFrequency(), SPI_MSBFIRST, SPI_MODE0));
				SPI.writeBytes(data, length);
				SPI.endTransaction();
			}

		private:
	};

	class ArduinoI2CHal : public I2CHal {
		public:
			void setup(uint8_t sdaPin, uint8_t sclPin, uint32_t frequency = 0) override {
				Wire.begin(sdaPin, sclPin, frequency);
			}

			uint8_t readByte(uint16_t slaveAddress, uint8_t registerAddress) override {
				uint8_t result = 0;
				uint8_t readLength;

				do {
					Wire.beginTransmission(slaveAddress);
					Wire.write(registerAddress);
					Wire.endTransmission(false);
					// Restart
					// getHal()->delayMilliseconds(10);
					readLength = Wire.requestFrom(slaveAddress, (uint8_t) 1);
				}
				while (readLength == 0);

				while (Wire.available()) {
					result = Wire.read();
				}

				return result;
			}

			void writeByte(uint16_t slaveAddress, uint8_t registerAddress, uint8_t data) override {
				Wire.beginTransmission(slaveAddress);
				Wire.write(registerAddress);
				Wire.write(data);
				Wire.endTransmission();
			}
	};

	class ArduinoHal : public MCUHal {
		public:
			ArduinoHal() : MCUHal(&_GPIO, &_SPI, &_I2C) {

			}

			void sleep(uint32_t milliseconds) override {
				delay(milliseconds);
			}

			uint32_t getTime() override {
				return millis();
			}

		private:
			ArduinoGPIOHal _GPIO;
			ArduinoSPIHal _SPI;
			ArduinoI2CHal _I2C;
	};
}