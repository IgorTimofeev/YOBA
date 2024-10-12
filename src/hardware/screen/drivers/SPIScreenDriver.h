#pragma once

#include <cstdint>
#include "SPI.h"
#include "screenDriver.h"

namespace yoba {
	class SPIScreenDriver : public TypedScreenDriver<ColorType> {
		public:
			SPIScreenDriver(
				ColorType colorType,
				const Size& resolution,
				ScreenOrientation orientation,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency
			);

			void setup() override;

			uint8_t getTransactionWindowHeight() const;

			uint8_t* getTransactionBuffer() const;
			size_t getTransactionBufferLength() const;

			/* To send a set of lines we have to send a command, 2 data bytes, another command, 2 more data bytes and another command
			* before sending the line data itself; a total of 6 transactions. (We can't put all of this in just one transaction
			* because the D/C line needs to be toggled in the middle.)
			* This routine queues these commands up as interrupt transactions so they get
			* sent faster (compared to calling spi_device_transmit several times), and at
			* the mean while the lines for next transactions can get calculated.
			*/
			void flushTransactionBuffer(uint16_t y);

			void writePixels(const std::function<uint32_t(size_t pixelIndex)>& pixelGetter) override;

		protected:
			uint8_t _csPin;
			uint8_t _dcPin;
			int8_t _rstPin;

			uint8_t _transactionWindowHeight = 20;
			size_t _transactionBufferLength = 0;
			uint8_t* _transactionBuffer = nullptr;
			const SPISettings _spiSettings;

			void onOrientationChanged() override;

			void updateDataFromOrientation() override;

			// Driver sends pixel data to screen using tiny sliding window, moving it from
			// top to bottom until complete. This method defines how big window should be.
			// Larger value means more memory use, but less overhead for data transfer.
			//
			// [Fucking important] Make sure screen height is dividable by this.
			//
			// Example: let transactionWindowHeight be 40 px for 320 x 240 px screen.
			// This will allocate 320 * 40 = 25600 pixels * 2 bytes per each = 25 KiB of heap,
			// allowing screen buffer to be flushed in 240 / 40 = 6 equal parts,
			// which more than enough to achieve ~800 FPS on simple scenes on 240 MHz ESP32.
			// Increasing buffer height to 48/60/80/120 px can afford you 10-200 extra FPS,
			// but will eat RAM like a bulimic bitch
			virtual uint8_t getTransactionWindowHeightForOrientation() = 0;

			virtual void writeBeginCommands() = 0;
			virtual void writeOrientationChangeCommands() = 0;

			void writeData(uint8_t data);
			void writeData(const uint8_t *data, int length);

			void writeCommand(uint8_t data);

			void writeCommandAndData(uint8_t command, const uint8_t *data, int length);
			void writeCommandAndData(uint8_t command, uint8_t data);

		private:
			void setChipSelect(uint8_t value) const;

			void setDataCommand(uint8_t value) const;
	};

	template<ColorType ColorType>
	SPIScreenDriver<ColorType>::SPIScreenDriver(
		const Size& resolution,
		ScreenOrientation orientation,

		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		TypedScreenDriver<ColorType>(
			resolution,
			orientation
		),
		_csPin(csPin),
		_dcPin(dcPin),
		_rstPin(rstPin),
		_spiSettings(SPISettings(SPIFrequency, SPI_MSBFIRST, SPI_MODE0))
	{

	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::setup() {
		TypedScreenDriver<ColorType>::setup();

		// Resetting CS pin just in case
		pinMode(_csPin, OUTPUT);
		setChipSelect(true);

		//Initialize non-SPI GPIOs
		pinMode(_dcPin, OUTPUT);

		// Toggle reset pin if it was defined
		if (_rstPin >= 0) {
			pinMode(_rstPin, OUTPUT);

			digitalWrite(_rstPin, 0);
			vTaskDelay(100 / portTICK_PERIOD_MS);

			digitalWrite(_rstPin, 1);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		// SPI
		SPI.begin();

		writeBeginCommands();
	}

	template<ColorType ColorType>
	uint8_t SPIScreenDriver<ColorType>::getTransactionWindowHeight() const {
		return _transactionWindowHeight;
	}

	template<ColorType ColorType>
	inline void SPIScreenDriver<ColorType>::writeData(uint8_t data) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transfer(data);
		SPI.endTransaction();

		setChipSelect(true);
	}

	template<ColorType ColorType>
	inline void SPIScreenDriver<ColorType>::writeData(const uint8_t* data, int length) {
		setChipSelect(false);

		SPI.beginTransaction(_spiSettings);
		SPI.transferBytes(data, nullptr, length);
		SPI.endTransaction();

		setChipSelect(true);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::writeCommand(uint8_t data) {
		setDataCommand(false);
		writeData(data);
		setDataCommand(true);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::writeCommandAndData(uint8_t command, uint8_t data) {
		writeCommand(command);
		writeData(data);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::writeCommandAndData(uint8_t command, const uint8_t *data, int length) {
		writeCommand(command);
		writeData(data, length);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::flushTransactionBuffer(uint16_t y) {
		uint8_t data[4];

		// Column Address Set
		data[0] = 0; //Start Col High
		data[1] = 0; //Start Col Low
		data[2] = (this->_resolution.getWidth() - 1) >> 8; //End Col High
		data[3] = (this->_resolution.getWidth() - 1) & 0xff; //End Col Low
		writeCommandAndData(0x2A, data, 4);

		//Page address set
		data[0] = y >> 8; //Start page high
		data[1] = y & 0xff; // Start page low
		data[2] = (y + _transactionWindowHeight - 1) >> 8; // End page high
		data[3] = (y + _transactionWindowHeight - 1) & 0xff; // End page low
		writeCommandAndData(0x2B, data, 4);

		// Memory write
		writeCommandAndData(0x2C, (uint8_t*) _transactionBuffer, _transactionBufferLength);
	}

	template<ColorType ColorType>
	uint8_t* SPIScreenDriver<ColorType>::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	template<ColorType ColorType>
	size_t SPIScreenDriver<ColorType>::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::updateDataFromOrientation() {
		TypedScreenDriver<ColorType>::updateDataFromOrientation();

		// Updating transaction buffer height
		_transactionWindowHeight = getTransactionWindowHeightForOrientation();

		// Allocating transaction buffer
		delete _transactionBuffer;
		_transactionBufferLength = Color::getBytesForPixelsPerType(this->_resolution.getWidth() * _transactionWindowHeight, ColorType);
		_transactionBuffer = (uint8_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
		assert(_transactionBuffer != nullptr);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::onOrientationChanged() {
		TypedScreenDriver<ColorType>::onOrientationChanged();

		writeOrientationChangeCommands();
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::setChipSelect(uint8_t value) const {
		digitalWrite(_csPin, value);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::setDataCommand(uint8_t value) const {
		digitalWrite(_dcPin, value);
	}

	template<ColorType ColorType>
	void SPIScreenDriver<ColorType>::writePixels(const std::function<uint32_t(size_t pixelIndex)>& pixelGetter) {
		switch (ColorType) {
			case ColorType::Rgb565:
				const size_t pixelCount = this->_resolution.getWidth() * _transactionWindowHeight;
				size_t pixelIndex = 0;

				for (uint16_t y = 0; y < this->_resolution.getHeight(); y += _transactionWindowHeight) {
					for (size_t transactionBufferIndex = 0; transactionBufferIndex < pixelCount; transactionBufferIndex++) {
						_transactionBuffer[transactionBufferIndex] = pixelGetter(pixelIndex);
						pixelIndex++;
					}

					flushTransactionBuffer(y);
				}
				break;
		}
	}
}