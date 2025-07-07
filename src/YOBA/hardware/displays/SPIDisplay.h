#pragma once

#include <cstdint>
#include <YOBA/hardware/displays/display.h>

namespace YOBA {
	class SPIDisplay : public virtual Display {
		public:
			SPIDisplay(
				uint8_t mosiPin,
				uint8_t misoPin,
				uint8_t sckPin,
				int8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t frequency
			);

			~SPIDisplay() override = default;

			void setup() override;

		protected:
			uint8_t mosiPin;
			uint8_t misoPin;
			uint8_t sckPin;
			int8_t ssPin;
			uint8_t dcPin;
			int8_t rstPin;
			uint32_t frequency;

			void setDataCommandPin(bool value) const;
			void setResetPin(bool value) const;

			void writeData(uint8_t data);
			void writeData(const uint8_t *data, size_t length);

			void writeCommand(uint8_t command);

			void writeCommandAndData(uint8_t command, const uint8_t *data, size_t length);
			void writeCommandAndData(uint8_t command, uint8_t data);

			virtual void toggleResetPin();

	};
}