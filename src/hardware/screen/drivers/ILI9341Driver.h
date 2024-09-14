#pragma once

#include <cstdint>
#include "screenDriver.h"

namespace yoba {
	class ILI9341Driver : public ScreenDriver {
		public:
			explicit ILI9341Driver(uint8_t chipSelectPin, uint8_t dataCommandPin, int8_t resetPin);

			void writeInitializationCommands() override;

		private:
	};
}