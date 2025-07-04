#include "GC9A01Display.h"
#include <YOBA/system.h>
#include <cstdint>

namespace YOBA {
	GC9A01Display::GC9A01Display(
		const uint8_t mosiPin,
		const uint8_t misoPin,
		const uint8_t sckPin,
		const uint8_t ssPin,
		const uint8_t dcPin,
		const int8_t rstPin,
		const uint32_t SPIFrequency
	) :
		RenderTarget(
			Size(240, 240),
			ViewportRotation::clockwise0,
			PixelOrder::XY,
			ColorModel::RGB565
		),
		SPIDisplay(
			mosiPin,
			misoPin,
			sckPin,
			ssPin,
			dcPin,
			rstPin,
			SPIFrequency
		)
	{

	}

	void GC9A01Display::setup() {
		SPIDisplay::setup();

		/* Initial Sequence */

	    writeCommand(0xEF);

	    writeCommand(0xEB);
	    writeData(0x14);

	    writeCommand(0xFE);
	    writeCommand(0xEF);

	    writeCommand(0xEB);
	    writeData(0x14);

	    writeCommand(0x84);
	    writeData(0x40);

	    writeCommand(0x85);
	    writeData(0xFF);

	    writeCommand(0x86);
	    writeData(0xFF);

	    writeCommand(0x87);
	    writeData(0xFF);

	    writeCommand(0x88);
	    writeData(0x0A);

	    writeCommand(0x89);
	    writeData(0x21);

	    writeCommand(0x8A);
	    writeData(0x00);

	    writeCommand(0x8B);
	    writeData(0x80);

	    writeCommand(0x8C);
	    writeData(0x01);

	    writeCommand(0x8D);
	    writeData(0x01);

	    writeCommand(0x8E);
	    writeData(0xFF);

	    writeCommand(0x8F);
	    writeData(0xFF);


	    writeCommand(0xB6);
	    writeData(0x00);
	    writeData(0x00);

		writeMADCTLCommand();

	//     writeCommand(0x36);
	//
	// #if ORIENTATION == 0
	//     writeData(0x18);
	// #elif ORIENTATION == 1
	//     writeData(0x28);
	// #elif ORIENTATION == 2
	//     writeData(0x48);
	// #else
	//     writeData(0x88);
	// #endif

	    writeCommand(COLOR_MODE);
	    writeData(COLOR_MODE__16_BIT);

	    writeCommand(0x90);
	    writeData(0x08);
	    writeData(0x08);
	    writeData(0x08);
	    writeData(0x08);

	    writeCommand(0xBD);
	    writeData(0x06);

	    writeCommand(0xBC);
	    writeData(0x00);

	    writeCommand(0xFF);
	    writeData(0x60);
	    writeData(0x01);
	    writeData(0x04);

	    writeCommand(0xC3);
	    writeData(0x13);
	    writeCommand(0xC4);
	    writeData(0x13);

	    writeCommand(0xC9);
	    writeData(0x22);

	    writeCommand(0xBE);
	    writeData(0x11);

	    writeCommand(0xE1);
	    writeData(0x10);
	    writeData(0x0E);

	    writeCommand(0xDF);
	    writeData(0x21);
	    writeData(0x0c);
	    writeData(0x02);

	    writeCommand(0xF0);
	    writeData(0x45);
	    writeData(0x09);
	    writeData(0x08);
	    writeData(0x08);
	    writeData(0x26);
	    writeData(0x2A);

	    writeCommand(0xF1);
	    writeData(0x43);
	    writeData(0x70);
	    writeData(0x72);
	    writeData(0x36);
	    writeData(0x37);
	    writeData(0x6F);

	    writeCommand(0xF2);
	    writeData(0x45);
	    writeData(0x09);
	    writeData(0x08);
	    writeData(0x08);
	    writeData(0x26);
	    writeData(0x2A);

	    writeCommand(0xF3);
	    writeData(0x43);
	    writeData(0x70);
	    writeData(0x72);
	    writeData(0x36);
	    writeData(0x37);
	    writeData(0x6F);

	    writeCommand(0xED);
	    writeData(0x1B);
	    writeData(0x0B);

	    writeCommand(0xAE);
	    writeData(0x77);

	    writeCommand(0xCD);
	    writeData(0x63);

	    writeCommand(0x70);
	    writeData(0x07);
	    writeData(0x07);
	    writeData(0x04);
	    writeData(0x0E);
	    writeData(0x0F);
	    writeData(0x09);
	    writeData(0x07);
	    writeData(0x08);
	    writeData(0x03);

	    writeCommand(0xE8);
	    writeData(0x34);

	    writeCommand(0x62);
	    writeData(0x18);
	    writeData(0x0D);
	    writeData(0x71);
	    writeData(0xED);
	    writeData(0x70);
	    writeData(0x70);
	    writeData(0x18);
	    writeData(0x0F);
	    writeData(0x71);
	    writeData(0xEF);
	    writeData(0x70);
	    writeData(0x70);

	    writeCommand(0x63);
	    writeData(0x18);
	    writeData(0x11);
	    writeData(0x71);
	    writeData(0xF1);
	    writeData(0x70);
	    writeData(0x70);
	    writeData(0x18);
	    writeData(0x13);
	    writeData(0x71);
	    writeData(0xF3);
	    writeData(0x70);
	    writeData(0x70);

	    writeCommand(0x64);
	    writeData(0x28);
	    writeData(0x29);
	    writeData(0xF1);
	    writeData(0x01);
	    writeData(0xF1);
	    writeData(0x00);
	    writeData(0x07);

	    writeCommand(0x66);
	    writeData(0x3C);
	    writeData(0x00);
	    writeData(0xCD);
	    writeData(0x67);
	    writeData(0x45);
	    writeData(0x45);
	    writeData(0x10);
	    writeData(0x00);
	    writeData(0x00);
	    writeData(0x00);

	    writeCommand(0x67);
	    writeData(0x00);
	    writeData(0x3C);
	    writeData(0x00);
	    writeData(0x00);
	    writeData(0x00);
	    writeData(0x01);
	    writeData(0x54);
	    writeData(0x10);
	    writeData(0x32);
	    writeData(0x98);

	    writeCommand(0x74);
	    writeData(0x10);
	    writeData(0x85);
	    writeData(0x80);
	    writeData(0x00);
	    writeData(0x00);
	    writeData(0x4E);
	    writeData(0x00);

	    writeCommand(0x98);
	    writeData(0x3e);
	    writeData(0x07);

	    writeCommand(0x35);
	    writeCommand(0x21);

		writeCommand(GC9A01A_GAMMA1);
		writeData(0x45);
		writeData(0x09);
		writeData(0x08);
		writeData(0x08);
		writeData(0x26);
		writeData(0x2a);

		writeCommand(GC9A01A_GAMMA2);
		writeData(0x43);
		writeData(0x70);
		writeData(0x72);
		writeData(0x36);
		writeData(0x37);
		writeData(0x6f);

		writeCommand(GC9A01A_GAMMA3);
		writeData(0x45);
		writeData(0x09);
		writeData(0x08);
		writeData(0x08);
		writeData(0x26);
		writeData(0x2a);

		writeCommand(GC9A01A_GAMMA4);
		writeData(0x43);
		writeData(0x70);
		writeData(0x72);
		writeData(0x36);
		writeData(0x37);
		writeData(0x6f);

	    writeCommand(0x11);
	    system::sleep(120);
	    writeCommand(0x29);
	    system::sleep(20);
	}

	void GC9A01Display::writeMADCTLCommand() {
		uint8_t data;

		switch (getRotation()) {
			case ViewportRotation::clockwise0:
				data = MADCTL_MX | MADCTL_BGR;
				break;
			case ViewportRotation::clockwise90:
				data = MADCTL_MV | MADCTL_BGR;
				break;
			case ViewportRotation::clockwise180:
				data = MADCTL_MY | MADCTL_BGR;
				break;
			case ViewportRotation::clockwise270:
				data = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
				break;
		}

		writeCommandAndData(GC9A01A_MADCTL, &data, 1);
	}

	void GC9A01Display::writePixels(const Bounds& bounds, uint8_t* source, size_t length) {
		uint8_t data[4];

		writeCommand(COL_ADDR_SET);
		data[0] = (bounds.getX() >> 8) & 0xFF;
		data[1] = bounds.getX() & 0xFF;
		data[2] = (bounds.getX2() >> 8) & 0xFF;
		data[3] = bounds.getX2() & 0xFF;
		writeData(data, sizeof(data));

		writeCommand(ROW_ADDR_SET);
		data[0] = (bounds.getY() >> 8) & 0xFF;
		data[1] = bounds.getY() & 0xFF;
		data[2] = (bounds.getY2() >> 8) & 0xFF;
		data[3] = bounds.getY2() & 0xFF;
		writeData(data, sizeof(data));

		// Memory write
		writeCommandAndData(MEM_WR, source, length);
	}
}