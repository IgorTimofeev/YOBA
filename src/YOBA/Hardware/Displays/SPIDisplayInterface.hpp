#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Hardware/Displays/DisplayInterface.hpp>

namespace YOBA {
	class SPIDisplayInterface : public DisplayInterface {
		public:
			void setup(
				const uint8_t MOSIPin,
				const uint8_t SCKPin,
				const int8_t SSPin,
				const int8_t DCPin,
				const int8_t RSTPin,

				const uint8_t bus,
				const uint8_t mode,
				const uint32_t frequencyHz
			);

			SPIDevice& getSPIDevice();
			int8_t getDCPin() const;

			bool write(const uint8_t data) override;
			bool write(const std::span<const uint8_t> data) override;
			bool writeCommand(const uint8_t command) override;

			void setDCPinState(const bool state) const;

		private:
			SPIDevice _SPIDevice {};

			int8_t _DCPin = -1;

			using DisplayInterface::setup;
	};
}

#endif