#pragma once

#include <cstdint>
#include "SPIScreenDriver.h"
#include "../../../size.h"

namespace yoba {
	class ILI9341Driver : public SPIScreenDriver {
		public:
			ILI9341Driver(
				ScreenOrientation orientation,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,

				// Somehow 40 MHz works nice on Arduino SPI, buf ESP-IDF handles only 26 MHz
				// Hmm...
				uint32_t SPIFrequency = 40000000
			);

		protected:
			void writeBeginCommands() override;
			void writeOrientationChangeCommands() override;
			uint8_t getTransactionWindowHeightForOrientation() override;

		private:
			enum class Command : uint8_t {
				/* MY, MX, MV, ML, BGR, MH, X, X */
				MADCTL = 0x36,
				MADCTL_MY = 0x80,
				MADCTL_MX = 0x40,
				MADCTL_MV = 0x20,
				MADCTL_ML = 0x10,
				MADCTL_BGR = 0x08,
				MADCTL_MH = 0x04,
			};
	};
}