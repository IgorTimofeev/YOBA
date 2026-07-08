#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_MCU)

#include <cstdint>

#include <YOBA/Hardware/Displays/GC9A01Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void GC9A01Display::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequency,

		const Size& size,
		const Rotation rotation,
		const ColorModel colorModel
	) {
		SPIDisplay::setup(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			RSTPin,
			SPIFrequency,

			size,
			rotation,
			PixelOrder::XNormalYNormal,
			colorModel
		);

		// Software reset
		if (_RSTPin < 0) {
			writeCommand(SWRESET);
			system::delayMs(150);
		}

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
	    writeData(getColorModel() == ColorModel::RGB565 ? COLOR_MODE__16_BIT : COLOR_MODE__18_BIT);

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

		writeCommand(GAMMA1);
		writeData(0x45);
		writeData(0x09);
		writeData(0x08);
		writeData(0x08);
		writeData(0x26);
		writeData(0x2a);

		writeCommand(GAMMA2);
		writeData(0x43);
		writeData(0x70);
		writeData(0x72);
		writeData(0x36);
		writeData(0x37);
		writeData(0x6f);

		writeCommand(GAMMA3);
		writeData(0x45);
		writeData(0x09);
		writeData(0x08);
		writeData(0x08);
		writeData(0x26);
		writeData(0x2a);

		writeCommand(GAMMA4);
		writeData(0x43);
		writeData(0x70);
		writeData(0x72);
		writeData(0x36);
		writeData(0x37);
		writeData(0x6f);

	    writeCommand(SLPOUT);
	    system::delayMs(120);
	}

	void GC9A01Display::writeMADCTLCommand() {
		uint8_t data = 0;

		switch (getRotation()) {
			case Rotation::none:
				data = MADCTL_MX | MADCTL_BGR;
				break;
			case Rotation::clockwise90:
				data = MADCTL_MV | MADCTL_BGR;
				break;
			case Rotation::clockwise180:
				data = MADCTL_MY | MADCTL_BGR;
				break;
			case Rotation::clockwise270:
				data = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
				break;
		}

		writeCommand(MADCTL);
		writeData(data);
	}

	void GC9A01Display::writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		writeCommand(COL_ADDR_SET);
		data[0] = (bounds.getX() >> 8) & 0xFF;
		data[1] = bounds.getX() & 0xFF;
		data[2] = (bounds.getX2() >> 8) & 0xFF;
		data[3] = bounds.getX2() & 0xFF;
		writeData({ data, 4 });

		writeCommand(ROW_ADDR_SET);
		data[0] = (bounds.getY() >> 8) & 0xFF;
		data[1] = bounds.getY() & 0xFF;
		data[2] = (bounds.getY2() >> 8) & 0xFF;
		data[3] = bounds.getY2() & 0xFF;
		writeData({ data, 4 });

		// Memory write
		writeCommand(MEM_WR);
		writeData(pixelBuffer);
	}

	void GC9A01Display::turnOn() {
		writeCommand(DISPON);
		system::delayMs(20);
	}

	void GC9A01Display::turnOff() {
		writeCommand(DISPOFF);
	}
}

#endif