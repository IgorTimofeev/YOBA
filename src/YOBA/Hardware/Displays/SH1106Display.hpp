#pragma once

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_MCU)

#include <cstdint>
#include <span>

#include <YOBA/Hardware/Displays/SPIDisplay.hpp>
#include <YOBA/Hardware/Displays/ContrastDisplay.hpp>
#include <YOBA/Hardware/Displays/InvertibleDisplay.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	class SH1106Display : public SPIDisplay, public ContrastDisplay, public InvertibleDisplay {
		public:
			void setup(
				const uint8_t MOSIPin,
				const uint8_t SCKPin,
				const int8_t SSPin,
				const uint8_t DCPin,
				const int8_t RSTPin,
				const uint32_t SPIFrequencyHz
			);

			void flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override;
			void setContrast(uint8_t value) override;
			void setInverted(bool value) override;

		private:
			enum class Command : uint8_t {
				setContrast = 0x81,
				displayAllOnResume = 0xA4,
				displayAllOn = 0xA5,
				normalDisplay = 0xA6,
				invertDisplay = 0xA7,
				displayOff = 0xAE,
				displayOn = 0xAF,
				outputFollowsRam = 0xA4,

				setDisplayOffset = 0xD3,
				setComPins = 0xDA,

				setVComDetect = 0xDB,

				setDisplayClockDiv = 0xD5,
				setPrecharge = 0xD9,

				setMultiplex = 0xA8,

				setLowColumn = 0x00,
				setHighColumn = 0x10,
				setSegmentRemap = 0xA1,

				setStartLine = 0x40,

				memoryMode = 0x20,
				columnAddress = 0x21,
				pageAddress = 0x22,
				setPageAddress = 0xB0,
				setColumnAddressLow = 0x02,
				setColumnAddressHigh = 0x10,

				comScanInc = 0xC0,
				comScanDec = 0xC8,

				segremap = 0xA0,

				chargePump = 0x8D,
			};

			constexpr static uint8_t _pageCount = 8;
	};
}

#endif