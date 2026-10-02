#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Hardware/Displays/DisplayInterface.hpp>

namespace YOBA {
	class SPIDisplayInterface : public DisplayInterface {
		public:
			void setup(
				const uint8_t busIndex,
				const uint8_t mode,

				const uint8_t MOSIPin,
				const uint8_t SCKPin,
				const int8_t SSPin,
				const uint32_t frequencyHz,

				const int8_t DCPin,
				const int8_t RSTPin
			);

			system::SPIDevice& getSPIDevice();
			int8_t getDCPin() const;
			int8_t getRSTPin() const;

			bool write(const uint8_t data) override;
			bool write(const std::span<const uint8_t> data) override;
			bool writeCommand(const uint8_t command);

			void setDCPinState(const bool state) const;
			void setRSTPinState(const bool state) const;
			void toggleRSTPin(uint32_t delayAfterLowMs, uint32_t delayAfterHighMs) const;

		private:
			system::SPIDevice _SPIDevice {};

			int8_t _DCPin = -1;
			int8_t _RSTPin = -1;
	};
}

#endif