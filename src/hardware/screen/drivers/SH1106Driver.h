#pragma once

#include <cstdint>
#include "SPIScreenDriver.h"
#include "../../../size.h"

namespace yoba {
#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETPAGEADDRESS 0xB0
#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_READ_MODIFY_START 0xE0
#define SH1106_READ_MODIFY_END 0xEE

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

	class SH1106Driver : public SPIScreenDriver {
		public:
			SH1106Driver(
				ScreenOrientation orientation,
				Size resolution,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,

				uint32_t SPIFrequency = 8000000
			);

			void flushTransactionBuffer(uint16_t y) override;

		protected:
			void writeBeginCommands() override;
			void writeOrientationChangeCommands() override;
			void writeColorModeChangeCommands() override;

			uint8_t getTransactionWindowHeightForOrientation() override;

		private:
			enum class Command : uint8_t {
				COLMOD = 0x3A,

				/* MY, MX, MV, ML, BGR, MH, X, X */
				MADCTL = 0x36,
				MADCTL_MY = 0x80,
				MADCTL_MX = 0x40,
				MADCTL_MV = 0x20,
				MADCTL_ML = 0x10,
				MADCTL_BGR = 0x08,
				MADCTL_MH = 0x04,
			};
	};
}