#pragma once

#include <cstdint>
#include "screenDriver.h"
#include "../../../size.h"

namespace yoba {
	class ILI9341Driver : public ScreenDriver {
		public:
			explicit ILI9341Driver(
				uint8_t chipSelectPin,
				uint8_t dataCommandPin,
				int8_t resetPin,
				ScreenOrientation orientation
			);

			void writeInitializationCommands() override;

		private:
	};
}