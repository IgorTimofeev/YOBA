#include <cstdint>
#include "SH1106Driver.h"

namespace yoba {
	SH1106Driver::SH1106Driver(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		SPIScreenDriver(
			ScreenDriverBufferType::Full,
			ColorModel::Monochrome,
			Size(128, 64),
			ScreenOrientation::Clockwise0,

			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		)
	{

	}

	void SH1106Driver::writeOrientationChangeCommands() {

	}

	void SH1106Driver::writeColorModeChangeCommands() {

	}

	void SH1106Driver::writeBeginCommands() {
		const auto vccstate = SH1106_SWITCHCAPVCC;
		
		// Init sequence
		writeCommand(SH1106_DISPLAYOFF);                    // 0xAE
		writeCommand(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5

			// Init sequence for 128x64 OLED module
		writeCommand(0xF0);                                 // the suggested ratio 0xF0 XXXXXXXX
		writeCommand(SH1106_SETMULTIPLEX);                  // 0xA8 XXXXXXXXX
		writeCommand(0x3F);								   // XXXXXXXXXXX
		writeCommand(SH1106_OUTPUT_FOLLOWS_RAM);            // 0xA4 XXXXXXXXX
		writeCommand(SH1106_SETDISPLAYOFFSET);              // 0xD3 XXXXXXXXX
		writeCommand(0x0);                                  // no offset  XXXXXXXXX
		writeCommand(SH1106_SETSTARTLINE | 0x0);            // line #0
		writeCommand(SH1106_CHARGEPUMP);                    // 0x8D XXXXXXXXXX
		if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXXXX
		  { writeCommand(0x10); }						   // XXXXXXXXX
		else												   // XXXXXXXXX
		  { writeCommand(0x14); }						   // XXXXXXXXX
		writeCommand(SH1106_MEMORYMODE);                    // 0x20 XXXXXXXXXXXXXX
	//    writeCommand(PAGE);                                  // 0x2 Paged XXXXXXXXXXX
		writeCommand(HORIZONTAL);                                  // 0x0 Horizontal XXXXXXXXXXX
		writeCommand(SH1106_SET_PAGE_ADDRESS); // start at page address 0 XXXXXXXXX
	//    writeCommand(SH1106_SEGREMAP | 0x1);				   // ?????????????
		writeCommand(SH1106_COMSCANDEC);					   // XXXXXXXXXX
		writeCommand(SH1106_SETLOWCOLUMN);				   // XXXXXXXXXX
		writeCommand(SH1106_SETHIGHCOLUMN);				   // XXXXXXXXXX
		writeCommand(SH1106_SETCOMPINS);                    // 0xDA XXXXXXXXX
		writeCommand(0x12);								   // XXXXXXXXXX
		writeCommand(SH1106_SETCONTRAST);                   // 0x81 XXXXXXXX
		if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXX
		  { writeCommand(0x9F); }						   // XXXXXXX
		else					   							   // XXXXXXX
		  { writeCommand(0xCF); }					       // XXXXXXX
		writeCommand(SH1106_SET_SEGMENT_REMAP);             // 0xA1 XXXXXXXX
		writeCommand(SH1106_SETPRECHARGE);                  // 0xd9 XXXXXXXXX
		if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXXXX
		  { writeCommand(0x22); }						   // XXXXXXXXX
		else												   // XXXXXXXXX
		  { writeCommand(0xF1); }						   // XXXXXXXXX
		writeCommand(SH1106_SETVCOMDETECT);                 // 0xDB XXXXXXXX
		writeCommand(0x20);								   // 0.77xVcc XXXXXXX
		writeCommand(SH1106_DISPLAYALLON_RESUME);           // 0xA4 XXXXXXXXXX
		writeCommand(SH1106_NORMALDISPLAY);                 // 0xA6  XXXXXXXXXX

		writeCommand(SH1106_DISPLAYON);//--turn on oled panel
	}

	void SH1106Driver::writePixels(uint8_t* buffer) {
//		uint8_t buffer2[SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8] {};
//
//		buffer2[0] = 1;
//		buffer2[2] = 1;

		for (uint8_t page = 0; page < SH1106_MAX_PAGE_COUNT; page++)
		{
			writeCommand(SH1106_SET_PAGE_ADDRESS + page);
			writeCommand(0x02); // low column start address
			writeCommand(0x10); // high column start address

			for (uint16_t pixel = 0; pixel < SH1106_LCDWIDTH; pixel++)
			{
				writeData(buffer[(page << 7) + pixel]);  // Page * 128 + pixel
			}
		}
	}
}