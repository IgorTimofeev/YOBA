#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_ESP_IDF)

#include <cstdio>
#include <cstring>
#include <esp_timer.h>
#include <limits>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <sdkconfig.h>
#include <driver/spi_master.h>
#include <driver/i2c_master.h>
#include <esp_heap_caps.h>

namespace YOBA::system {
	// -------------------------------- System --------------------------------

	void delayMs(const uint32_t duration) {
		vTaskDelay(pdMS_TO_TICKS(duration));
	}

	uint64_t getTimeUs() {
		return static_cast<uint64_t>(esp_timer_get_time());
	}

	void reallocate(uint8_t*& buffer, const size_t length) {
		if (buffer)
			heap_caps_free(buffer);

		size_t alignment = 0;

		#if CONFIG_SPIRAM
			// Should give us safe maximum
			alignment = CONFIG_MMU_PAGE_SIZE;
		#else
			alignment = 4;
		#endif

		buffer = static_cast<uint8_t*>(heap_caps_aligned_alloc(alignment, length, MALLOC_CAP_DMA));
		assert(buffer != nullptr && "Failed to allocate memory");
	}

	// -------------------------------- GPIO --------------------------------

	void GPIO::setMode(const uint8_t pin, const PinMode mode) {
		gpio_config_t config {};
		config.pin_bit_mask = 1ULL << pin;
		config.mode = mode == PinMode::input ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT;
		config.pull_up_en = GPIO_PULLUP_ENABLE;
		config.pull_down_en = GPIO_PULLDOWN_DISABLE;
		config.intr_type = GPIO_INTR_DISABLE;

		gpio_config(&config);
	}

	bool GPIO::read(uint8_t pin) {
		return gpio_get_level(static_cast<gpio_num_t>(pin));
	}

	void GPIO::write(uint8_t pin, const bool value) {
		gpio_set_level(static_cast<gpio_num_t>(pin), value);
	}

	void GPIO::addInterruptHandler(uint8_t pin, void(*callback)(void* arg), void* args) {
		gpio_install_isr_service(0);
		gpio_set_intr_type(static_cast<gpio_num_t>(pin), GPIO_INTR_ANYEDGE);
		gpio_isr_handler_add(static_cast<gpio_num_t>(pin), callback, args);
	}

	// -------------------------------- SPI --------------------------------

	void SPIDevice::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const int8_t DCPin,

		const uint32_t frequencyHz
	) {
		_MOSIPin = MOSIPin;
		_SCKPin = SCKPin;
		_SSPin = SSPin;
		_DCPin = DCPin;
		_frequencyHz = frequencyHz;

		// GPIO
		GPIO::setMode(_SSPin, GPIO::PinMode::output);
		GPIO::write(_SSPin, true);

		// Bus
		spi_bus_config_t busConfig {};
		busConfig.mosi_io_num = _MOSIPin;
		busConfig.miso_io_num = -1;
		busConfig.sclk_io_num = _SCKPin;
		busConfig.quadwp_io_num = -1;
		busConfig.quadhd_io_num = -1;
		busConfig.max_transfer_sz = 0xFFFF;

		// May be already initialized
		const auto result = spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO);
		assert(result == ESP_OK || result == ESP_ERR_INVALID_STATE);

		// Interface
		spi_device_interface_config_t interfaceConfig {};
		interfaceConfig.mode = 0;
		interfaceConfig.clock_speed_hz = static_cast<int>(_frequencyHz);
		interfaceConfig.spics_io_num = static_cast<int>(_SSPin);
		interfaceConfig.flags = SPI_DEVICE_NO_DUMMY;
		interfaceConfig.queue_size = 1;

		// Data / command pin behavior
		if (_DCPin != GPIO_NUM_NC) {
			GPIO::setMode(_DCPin, GPIO::PinMode::output);
			GPIO::write(_DCPin, true);

			interfaceConfig.pre_cb = [](spi_transaction_t* transaction) {
				const auto device = static_cast<SPIDevice*>(transaction->user);

				if (device->_commandMode)
					GPIO::write(device->_DCPin, false);
			};

			interfaceConfig.post_cb = [](spi_transaction_t* transaction) {
				const auto device = static_cast<SPIDevice*>(transaction->user);

				if (device->_commandMode)
					GPIO::write(device->_DCPin, true);
			};
		}

		ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_deviceHandle));
	}

	bool SPIDevice::write(const uint8_t data) {
		spi_transaction_t transaction {};
		transaction.tx_data[0] = data;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.length = 8;
		transaction.user = this;

		return ESP_ERROR_CHECK_WITHOUT_ABORT(spi_device_transmit(_deviceHandle, &transaction)) == ESP_OK;
	}

	bool SPIDevice::write(const std::span<const uint8_t> data) {
		spi_transaction_t transaction {};
		transaction.length = data.size() * 8;
		transaction.tx_buffer = data.data();
		transaction.user = this;

		return ESP_ERROR_CHECK_WITHOUT_ABORT(spi_device_transmit(_deviceHandle, &transaction)) == ESP_OK;
	}

	void SPIDevice::setCommandMode(const bool value) {
		_commandMode = value;
	}

	// -------------------------------- I2C --------------------------------

	void I2CDevice::setup(
		const uint8_t SDAPin,
		const uint8_t SCLPin,
		const uint16_t address,
		const uint32_t frequencyHz
	) {
		_SCLPin = SCLPin;
		_SDAPin = SDAPin;
		_address = address;
		_frequencyHz = frequencyHz;

		i2c_master_bus_config_t busConfig {};
		busConfig.i2c_port = I2C_NUM_0;
		busConfig.sda_io_num = static_cast<gpio_num_t>(_SDAPin);
		busConfig.scl_io_num = static_cast<gpio_num_t>(_SCLPin);
		busConfig.clk_source = I2C_CLK_SRC_DEFAULT;
		busConfig.glitch_ignore_cnt = 7;
		busConfig.flags.enable_internal_pullup = true;
		busConfig.flags.allow_pd = false;

		// May be already initialized
		i2c_new_master_bus(&busConfig, &_busHandle);

		i2c_device_config_t deviceConfig {};
		deviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
		deviceConfig.device_address = _address;
		deviceConfig.scl_speed_hz = _frequencyHz;

		ESP_ERROR_CHECK(i2c_master_bus_add_device(_busHandle, &deviceConfig, &_deviceHandle));
	}

	bool I2CDevice::read(const std::span<uint8_t> data) const {
		return ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_receive(_deviceHandle, data.data(), data.size(), 1'000)) == ESP_OK;
	}

	bool I2CDevice::write(const std::span<const uint8_t> data) const {
		return ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_transmit(_deviceHandle, data.data(), data.size(), 1'000)) == ESP_OK;
	}
}

#endif