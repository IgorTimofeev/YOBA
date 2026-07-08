#pragma once

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_ESP_IDF)

#include <cstdint>
#include <functional>
#include <span>
#include <assert.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <driver/gpio.h>

#include <driver/spi_master.h>
#include <driver/i2c_master.h>

namespace YOBA::system {
	void delayMs(uint32_t duration);
	uint64_t getTimeUs();

	void reallocate(uint8_t*& buffer, const size_t length);

	class GPIO {
		public:
			enum class PinMode : uint8_t {
				input,
				output
			};

			static void setMode(uint8_t pin, PinMode mode);
			static bool read(uint8_t pin);
			static void write(uint8_t pin, bool value);
			static void addInterruptHandler(uint8_t pin, void(*callback)(void* arg), void* args);
	};

	class SPIDevice {
		public:
			void setup(
				const uint8_t MOSIPin,
				const uint8_t SCKPin,
				const int8_t SSPin,
				const int8_t DCPin,
				const uint32_t frequencyHz
			);

			bool write(const uint8_t data);
			bool write(const std::span<const uint8_t> data);

			void setCommandMode(const bool value);

		private:
			uint8_t _MOSIPin {};
			uint8_t _SCKPin {};
			int8_t _SSPin {};
			int8_t _DCPin {};
			uint32_t _frequencyHz {};

			spi_device_handle_t _deviceHandle {};

			bool _commandMode = false;
	};

	class I2CDevice {
		public:
			void setup(
				const uint8_t SDAPin,
				const uint8_t SCLPin,
				const uint16_t address,
				const uint32_t frequencyHz
			);

			bool read(const std::span<uint8_t> data) const;
			bool write(const std::span<const uint8_t> data) const;

		private:
			uint8_t _SCLPin = 0;
			uint8_t _SDAPin = 0;
			uint16_t _address = 0;
			uint32_t _frequencyHz = 0;

			i2c_master_bus_handle_t _busHandle {};
			i2c_master_dev_handle_t _deviceHandle {};
	};
}

#endif