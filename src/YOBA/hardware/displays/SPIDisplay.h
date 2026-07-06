#pragma once

#include <cstdint>
#include <span>

#include <YOBA/hardware/displays/display.h>
#include <YOBA/system.h>

namespace YOBA {
	class SPIDisplay : public virtual Display {
		public:
			SPIDisplay(
				uint8_t MOSIPin,
				uint8_t SCKPin,
				int8_t SSPin,
				uint8_t DCPin,
				int8_t RSTPin,
				uint32_t SPIFrequencyHz
			);

			~SPIDisplay() override = default;

			void setup() override;

		protected:
			system::SPIDevice _SPIDevice;
			int8_t _RSTPin;

			void setResetPin(bool value) const;

			void writeCommand(uint8_t command);
			void writeData(uint8_t data);
			void writeData(const std::span<uint8_t> data);

			virtual void toggleResetPin();

	};
}