#pragma once

#include "SPIScreenDriver.h"

namespace yoba {
	class TransactionalSPIScreenDriver : public SPIScreenDriver {
		public:
			TransactionalSPIScreenDriver(
				ColorModel colorModel,
				const Size& resolution,
				ScreenOrientation orientation,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency
			);

			void updateDataFromOrientation() override;

			uint8_t getTransactionWindowHeight() const;

			uint8_t* getTransactionBuffer() const;

			size_t getTransactionBufferLength() const;

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

			/* To send a set of lines we have to send a command, 2 data bytes, another command, 2 more data bytes and another command
			* before sending the line data itself; a total of 6 transactions. (We can't put all of this in just one transaction
			* because the D/C line needs to be toggled in the middle.)
			* This routine queues these commands up as interrupt transactions so they get
			* sent faster (compared to calling spi_device_transmit several times), and at
			* the mean while the lines for next transactions can get calculated.
			*/
			virtual void writeTransactionBuffer(uint16_t y);

			virtual void writeTransactionBuffer(const std::function<void(uint8_t*& destination, size_t& pixelIndex)>& pixelSetter);

		protected:
			uint8_t _transactionWindowHeight = 20;
			size_t _transactionBufferLength = 0;
			uint8_t* _transactionBuffer = nullptr;
	};
}
