#pragma once

#include <cstdint>
#include <span>

#include <YOBA/hardware/displays/display.h>
#include <YOBA/system.h>

namespace YOBA {
	class SPIDisplay : public virtual Display {
		public:
			SPIDisplay(
				uint8_t mosiPin,
				uint8_t sckPin,
				int8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t frequencyHz
			);

			~SPIDisplay() override = default;

			void setup() override;

		protected:
			system::SPIDevice _SPIDevice;
			int8_t _rstPin;

			void setResetPin(bool value) const;

			void writeCommand(uint8_t command);
			void writeData(uint8_t data);
			void writeData(const std::span<uint8_t> data);

			virtual void toggleResetPin();

	};
}