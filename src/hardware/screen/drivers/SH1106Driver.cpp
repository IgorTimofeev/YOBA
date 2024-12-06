#include <cstdint>
#include "SH1106Driver.h"

namespace yoba {
	SH1106Driver::SH1106Driver(
		ScreenOrientation orientation,
		Size resolution,

		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		SPIScreenDriver(
			ColorModel::Monochrome,
			resolution,
			orientation,

			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		)
	{

	}

	void SH1106Driver::writeOrientationChangeCommands() {

	}

	uint8_t SH1106Driver::getTransactionWindowHeightForOrientation() {
		return 64;
	}

	void SH1106Driver::writeColorModeChangeCommands() {

	}

	void SH1106Driver::writeBeginCommands() {
		SPI.setClockDivider (SPI_CLOCK_DIV2); // 8 MHz

		// Init sequence for 128x64 OLED module
		this->writeCommand(SH1106_DISPLAYOFF);
		this->writeCommandAndData(SH1106_SETDISPLAYCLOCKDIV, 0x80);// the suggested ratio 0x80
		this->writeCommandAndData(SH1106_SETMULTIPLEX, 0x3F);
		this->writeCommandAndData(SH1106_SETDISPLAYOFFSET, 0x00); // no offset

		this->writeCommand(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
		this->writeCommand(SH1106_CHARGEPUMP);                    // 0x8D

		// SH1106_EXTERNALVCC
		//	this->writeData(0x10)
		// SH1106_SWITCHCAPVCC
		this->writeData(0x14);

		this->writeCommandAndData(SH1106_MEMORYMODE, 0x00); // 0x0 act like ks0108
		this->writeCommand(SH1106_SEGREMAP | 0x1);
		this->writeCommand(SH1106_COMSCANDEC);
		this->writeCommandAndData(SH1106_SETCOMPINS, 0x12);
		this->writeCommand(SH1106_SETCONTRAST);                   // 0x81

		// SH1106_EXTERNALVCC
		//	this->writeData(0x9F)
		// SH1106_SWITCHCAPVCC
		this->writeData(0xCF);

		this->writeCommand(SH1106_SETPRECHARGE);                  // 0xd9

		// SH1106_EXTERNALVCC
		//	this->writeCommand(0x22)
		// SH1106_SWITCHCAPVCC
		this->writeData(0xF1);

		this->writeCommandAndData(SH1106_SETVCOMDETECT, 0x40);
		this->writeCommand(SH1106_DISPLAYALLON_RESUME);           // 0xA4
		this->writeCommand(SH1106_NORMALDISPLAY);                 // 0xA6
	}

	void SH1106Driver::flushTransactionBuffer(uint16_t y) {
//		uint8_t data[4];
//
//		// Column Address Set
//		data[0] = 0; //Start Col High
//		data[1] = 0; //Start Col Low
//		data[2] = (this->_resolution.getWidth() - 1) >> 8; //End Col High
//		data[3] = (this->_resolution.getWidth() - 1) & 0xff; //End Col Low
//		writeCommandAndData(0x2A, data, 4);

		writeCommand(SH1106_SETSTARTLINE | 0x0); // line #0

		for (uint8_t pageIndex = 0; pageIndex < 8; pageIndex++) {
			writeCommand(SH1106_SETPAGEADDRESS | pageIndex);//set page address
			writeCommand(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
			writeCommand(SH1106_SETHIGHCOLUMN | 0x0);  // hi col = 0

			writeCommand(SH1106_READ_MODIFY_START);

			for (int columnY = 0; columnY < 8; columnY++) {
				for (int columnX = 0; columnX < 16; columnX++) {
					writeData(0xFF);
				}
			}

			writeCommand(SH1106_READ_MODIFY_END);
		}
	}
}