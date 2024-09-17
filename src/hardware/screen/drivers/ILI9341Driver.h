#pragma once

#include <cstdint>
#include "screenDriver.h"
#include "../../../size.h"

namespace yoba {
	class ILI9341Driver : public ScreenDriver {
		public:
			explicit ILI9341Driver(
				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				ScreenOrientation orientation
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

			void writeMemoryAccessControl();
	};
}