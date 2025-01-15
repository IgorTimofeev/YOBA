#pragma once

#include <cstdint>
#include "SPIDisplay.h"
#include "directWritingDisplay.h"
#include "contrastDisplay.h"
#include "invertibleDisplay.h"
#include "size.h"

namespace yoba::hardware {
	class SH1106Display : public SPIDisplay, public DirectWritingDisplay, public ContrastDisplay, public InvertibleDisplay {
		public:
			SH1106Display(
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
			static const uint8_t pageCount = 8;

			enum class Command : uint8_t {
				SetContrast = 0x81,
				DisplayAllOnResume = 0xA4,
				DisplayAllOn = 0xA5,
				NormalDisplay = 0xA6,
				InvertDisplay = 0xA7,
				DisplayOff = 0xAE,
				DisplayOn = 0xAF,
				OutputFollowsRam = 0xA4,

				SetDisplayOffset = 0xD3,
				SetComPins = 0xDA,

				SetVComDetect = 0xDB,

				SetDisplayClockDiv = 0xD5,
				SetPrecharge = 0xD9,

				SetMultiplex = 0xA8,

				SetLowColumn = 0x00,
				SetHighColumn = 0x10,
				SetSegmentRemap = 0xA1,

				SetStartLine = 0x40,

				MemoryMode = 0x20,
				ColumnAddress = 0x21,
				PageAddress = 0x22,
				SetPageAddress = 0xB0,
				SetColumnAddressLow = 0x02,
				SetColumnAddressHigh = 0x10,

				ComScanInc = 0xC0,
				ComScanDec = 0xC8,

				Segremap = 0xA0,

				ChargePump = 0x8D,
			};
	};
}