#pragma once

#include <cstdint>
#include "SPIScreenDriver.h"
#include "directScreenDriver.h"
#include "contrastScreenDriver.h"
#include "invertibleScreenDriver.h"
#include "../../../size.h"

namespace yoba {
	class ST7565Driver : public SPIScreenDriver, public DirectScreenDriver, public ContrastScreenDriver, public InvertibleScreenDriver {
		public:
			ST7565Driver(
				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,

				uint32_t SPIFrequency = 8000000
			);

			void writePixels(uint8_t* buffer) override;
			void setContrast(uint8_t value) override;

			void setInverted(bool value) override;

		protected:
			void writeSetupCommands() override;
			void writeOrientationChangeCommands() override;
			void writeColorModeChangeCommands() override;

		private:
			enum class Command : uint8_t {
				DISPLAY_OFF = 0xAE,
				DISPLAY_ON = 0xAF,

				SET_DISP_START_LINE = 0x40,
				SET_PAGE = 0xB0,

				SET_COLUMN_UPPER = 0x10,
				SET_COLUMN_LOWER = 0x00,

				SET_ADC_NORMAL = 0xA0,
				SET_ADC_REVERSE = 0xA1,

				SET_DISP_NORMAL = 0xA6,
				SET_DISP_REVERSE = 0xA7,

				SET_ALLPTS_NORMAL = 0xA4,
				SET_ALLPTS_ON = 0xA5,
				SET_BIAS_9 = 0xA2,
				SET_BIAS_7 = 0xA3,

				RMW = 0xE0,
				RMW_CLEAR = 0xEE,
				INTERNAL_RESET = 0xE2,
				SET_COM_NORMAL = 0xC0,
				SET_COM_REVERSE = 0xC8,
				SET_POWER_CONTROL = 0x28,
				SET_RESISTOR_RATIO = 0x20,
				SET_VOLUME_FIRST = 0x81,
				SET_VOLUME_SECOND = 0,
				SET_STATIC_OFF = 0xAC,
				SET_STATIC_ON = 0xAD,
				SET_STATIC_REG = 0x0,
				SET_BOOSTER_FIRST = 0xF8,
				SET_BOOSTER_234 = 0,
				SET_BOOSTER_5 = 1,
				SET_BOOSTER_6 = 3,
				NOP = 0xE3,
				TEST = 0xF0,
			};
	};
}