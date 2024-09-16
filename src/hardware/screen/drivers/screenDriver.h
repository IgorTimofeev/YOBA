#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "point.h"

namespace yoba {
	enum class ScreenOrientation : uint8_t {
		Portrait0,
		Landscape90,
		Portrait180,
		Landscape270,
	};

	class ScreenDriver {
		public:
			explicit ScreenDriver(
				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				const Size& defaultSize,
				ScreenOrientation orientation
			);

			void begin();

			const Size &getSize() const;

			ScreenOrientation getOrientation() const;
			virtual void setOrientation(ScreenOrientation orientation);

			uint8_t getTransactionBufferHeight() const;

			// Driver sends pixels to screen via SPI using small transaction buffer.
			// This method defines how big it should be. Larger value means more memory use,
			// but less overhead for data transfer. Make sure screen height is dividable by this.
			//
			// For example, let transactionBufferHeight be 40px for 320x240px screen
			// This will allocate 320 * 40 = 25600 pixels * 2 bytes per each = 25 KiB of heap,
			// allowing screen buffer to be flushed in 240 / 40 = 6 equal parts,
			// which more than enough to achieve ~800 FPS on simple scenes on 240 MHz ESP32.
			// Increasing this to 48/60/80/120 can afford you 10-100 extra FPS, but will
			// eat RAM like a bulimic bitch
			virtual uint8_t getTransactionBufferHeightForOrientation() = 0;

			virtual void rotatePoint(Point& point);

			virtual void writeInitializationCommands();

			/* Send a command to the LCD. Uses spi_device_polling_transmit, which waits
			* until the transfer is complete.
			*
			* Since command transactions are usually small, they are handled in polling
			* mode for higher speed. The overhead of interrupt transactions is more than
			* just waiting for the transaction to complete.
			*/
			void sendCommand(uint8_t command);

			/* Send data to the LCD. Uses spi_device_polling_transmit, which waits until the
			* transfer is complete.
			*
			* Since data transactions are usually small, they are handled in polling
			* mode for higher speed. The overhead of interrupt transactions is more than
			* just waiting for the transaction to complete.
			*/
			void sendData(const uint8_t *data, int length);

			void sendCommandAndData(uint8_t command, const uint8_t *data, int length);
			void sendCommandAndData(uint8_t command, uint8_t data);

			// This function is called (in irq context!) just before a transmission starts. It will
			// set the D/C line to the value indicated in the user field.
			static void SPIPreCallback(spi_transaction_t* transaction);

			/* To send a set of lines we have to send a command, 2 data bytes, another command, 2 more data bytes and another command
			* before sending the line data itself; a total of 6 transactions. (We can't put all of this in just one transaction
			* because the D/C line needs to be toggled in the middle.)
			* This routine queues these commands up as interrupt transactions so they get
			* sent faster (compared to calling spi_device_transmit several times), and at
			* the mean while the lines for next transactions can get calculated.
			*/
			void flushTransactionBuffer(int y);

			uint16_t *getTransactionBuffer() const;
			size_t getTransactionBufferLength() const;

			int32_t getSPIFrequency() const;
			void setSPIFrequency(int32_t spiFrequency);

		protected:
			uint8_t _csPin;
			uint8_t _dcPin;
			int8_t _rstPin;

			const Size _defaultSize;
			Size _size;
			ScreenOrientation _orientation;

			uint8_t _transactionBufferHeight = 20;
			size_t _transactionBufferLength = 0;
			uint16_t* _transactionBuffer = nullptr;

			spi_device_handle_t _spi = spi_device_handle_t();
			int32_t _spiFrequency = SPI_MASTER_FREQ_20M;

			void updateDataFromOrientation();
	};

	struct DriverSPIPreCallbackUserData {
		DriverSPIPreCallbackUserData(ScreenDriver *driver, bool dcPinState);

		ScreenDriver* driver;
		bool dcPinState;
	};
}