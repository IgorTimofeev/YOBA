#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/GC9A01Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void GC9A01Display::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const int8_t DCPin,
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
			_SPIDevice.writeCommand(SWRESET);
			system::delayMs(150);
		}

	    _SPIDevice.writeCommand(0xEF);

	    _SPIDevice.writeCommand(0xEB);
	    _SPIDevice.write(0x14);

	    _SPIDevice.writeCommand(0xFE);
	    _SPIDevice.writeCommand(0xEF);

	    _SPIDevice.writeCommand(0xEB);
	    _SPIDevice.write(0x14);

	    _SPIDevice.writeCommand(0x84);
	    _SPIDevice.write(0x40);

	    _SPIDevice.writeCommand(0x85);
	    _SPIDevice.write(0xFF);

	    _SPIDevice.writeCommand(0x86);
	    _SPIDevice.write(0xFF);

	    _SPIDevice.writeCommand(0x87);
	    _SPIDevice.write(0xFF);

	    _SPIDevice.writeCommand(0x88);
	    _SPIDevice.write(0x0A);

	    _SPIDevice.writeCommand(0x89);
	    _SPIDevice.write(0x21);

	    _SPIDevice.writeCommand(0x8A);
	    _SPIDevice.write(0x00);

	    _SPIDevice.writeCommand(0x8B);
	    _SPIDevice.write(0x80);

	    _SPIDevice.writeCommand(0x8C);
	    _SPIDevice.write(0x01);

	    _SPIDevice.writeCommand(0x8D);
	    _SPIDevice.write(0x01);

	    _SPIDevice.writeCommand(0x8E);
	    _SPIDevice.write(0xFF);

	    _SPIDevice.writeCommand(0x8F);
	    _SPIDevice.write(0xFF);

	    _SPIDevice.writeCommand(0xB6);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);

		writeMADCTLCommand();

	//     _SPIDevice.writeCommand(0x36);
	//
	// #if ORIENTATION == 0
	//     _SPIDevice.write(0x18);
	// #elif ORIENTATION == 1
	//     _SPIDevice.write(0x28);
	// #elif ORIENTATION == 2
	//     _SPIDevice.write(0x48);
	// #else
	//     _SPIDevice.write(0x88);
	// #endif

	    _SPIDevice.writeCommand(COLOR_MODE);
	    _SPIDevice.write(getColorModel() == ColorModel::RGB565 ? COLOR_MODE__16_BIT : COLOR_MODE__18_BIT);

	    _SPIDevice.writeCommand(0x90);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x08);

	    _SPIDevice.writeCommand(0xBD);
	    _SPIDevice.write(0x06);

	    _SPIDevice.writeCommand(0xBC);
	    _SPIDevice.write(0x00);

	    _SPIDevice.writeCommand(0xFF);
	    _SPIDevice.write(0x60);
	    _SPIDevice.write(0x01);
	    _SPIDevice.write(0x04);

	    _SPIDevice.writeCommand(0xC3);
	    _SPIDevice.write(0x13);
	    _SPIDevice.writeCommand(0xC4);
	    _SPIDevice.write(0x13);

	    _SPIDevice.writeCommand(0xC9);
	    _SPIDevice.write(0x22);

	    _SPIDevice.writeCommand(0xBE);
	    _SPIDevice.write(0x11);

	    _SPIDevice.writeCommand(0xE1);
	    _SPIDevice.write(0x10);
	    _SPIDevice.write(0x0E);

	    _SPIDevice.writeCommand(0xDF);
	    _SPIDevice.write(0x21);
	    _SPIDevice.write(0x0c);
	    _SPIDevice.write(0x02);

	    _SPIDevice.writeCommand(0xF0);
	    _SPIDevice.write(0x45);
	    _SPIDevice.write(0x09);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x26);
	    _SPIDevice.write(0x2A);

	    _SPIDevice.writeCommand(0xF1);
	    _SPIDevice.write(0x43);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x72);
	    _SPIDevice.write(0x36);
	    _SPIDevice.write(0x37);
	    _SPIDevice.write(0x6F);

	    _SPIDevice.writeCommand(0xF2);
	    _SPIDevice.write(0x45);
	    _SPIDevice.write(0x09);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x26);
	    _SPIDevice.write(0x2A);

	    _SPIDevice.writeCommand(0xF3);
	    _SPIDevice.write(0x43);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x72);
	    _SPIDevice.write(0x36);
	    _SPIDevice.write(0x37);
	    _SPIDevice.write(0x6F);

	    _SPIDevice.writeCommand(0xED);
	    _SPIDevice.write(0x1B);
	    _SPIDevice.write(0x0B);

	    _SPIDevice.writeCommand(0xAE);
	    _SPIDevice.write(0x77);

	    _SPIDevice.writeCommand(0xCD);
	    _SPIDevice.write(0x63);

	    _SPIDevice.writeCommand(0x70);
	    _SPIDevice.write(0x07);
	    _SPIDevice.write(0x07);
	    _SPIDevice.write(0x04);
	    _SPIDevice.write(0x0E);
	    _SPIDevice.write(0x0F);
	    _SPIDevice.write(0x09);
	    _SPIDevice.write(0x07);
	    _SPIDevice.write(0x08);
	    _SPIDevice.write(0x03);

	    _SPIDevice.writeCommand(0xE8);
	    _SPIDevice.write(0x34);

	    _SPIDevice.writeCommand(0x62);
	    _SPIDevice.write(0x18);
	    _SPIDevice.write(0x0D);
	    _SPIDevice.write(0x71);
	    _SPIDevice.write(0xED);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x18);
	    _SPIDevice.write(0x0F);
	    _SPIDevice.write(0x71);
	    _SPIDevice.write(0xEF);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x70);

	    _SPIDevice.writeCommand(0x63);
	    _SPIDevice.write(0x18);
	    _SPIDevice.write(0x11);
	    _SPIDevice.write(0x71);
	    _SPIDevice.write(0xF1);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x18);
	    _SPIDevice.write(0x13);
	    _SPIDevice.write(0x71);
	    _SPIDevice.write(0xF3);
	    _SPIDevice.write(0x70);
	    _SPIDevice.write(0x70);

	    _SPIDevice.writeCommand(0x64);
	    _SPIDevice.write(0x28);
	    _SPIDevice.write(0x29);
	    _SPIDevice.write(0xF1);
	    _SPIDevice.write(0x01);
	    _SPIDevice.write(0xF1);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x07);

	    _SPIDevice.writeCommand(0x66);
	    _SPIDevice.write(0x3C);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0xCD);
	    _SPIDevice.write(0x67);
	    _SPIDevice.write(0x45);
	    _SPIDevice.write(0x45);
	    _SPIDevice.write(0x10);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);

	    _SPIDevice.writeCommand(0x67);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x3C);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x01);
	    _SPIDevice.write(0x54);
	    _SPIDevice.write(0x10);
	    _SPIDevice.write(0x32);
	    _SPIDevice.write(0x98);

	    _SPIDevice.writeCommand(0x74);
	    _SPIDevice.write(0x10);
	    _SPIDevice.write(0x85);
	    _SPIDevice.write(0x80);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x00);
	    _SPIDevice.write(0x4E);
	    _SPIDevice.write(0x00);

	    _SPIDevice.writeCommand(0x98);
	    _SPIDevice.write(0x3e);
	    _SPIDevice.write(0x07);

	    _SPIDevice.writeCommand(0x35);
	    _SPIDevice.writeCommand(0x21);

		_SPIDevice.writeCommand(GAMMA1);
		_SPIDevice.write(0x45);
		_SPIDevice.write(0x09);
		_SPIDevice.write(0x08);
		_SPIDevice.write(0x08);
		_SPIDevice.write(0x26);
		_SPIDevice.write(0x2a);

		_SPIDevice.writeCommand(GAMMA2);
		_SPIDevice.write(0x43);
		_SPIDevice.write(0x70);
		_SPIDevice.write(0x72);
		_SPIDevice.write(0x36);
		_SPIDevice.write(0x37);
		_SPIDevice.write(0x6f);

		_SPIDevice.writeCommand(GAMMA3);
		_SPIDevice.write(0x45);
		_SPIDevice.write(0x09);
		_SPIDevice.write(0x08);
		_SPIDevice.write(0x08);
		_SPIDevice.write(0x26);
		_SPIDevice.write(0x2a);

		_SPIDevice.writeCommand(GAMMA4);
		_SPIDevice.write(0x43);
		_SPIDevice.write(0x70);
		_SPIDevice.write(0x72);
		_SPIDevice.write(0x36);
		_SPIDevice.write(0x37);
		_SPIDevice.write(0x6f);

	    _SPIDevice.writeCommand(SLPOUT);
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

		_SPIDevice.writeCommand(MADCTL);
		_SPIDevice.write(data);
	}

	void GC9A01Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		_SPIDevice.writeCommand(COL_ADDR_SET);
		data[0] = (bounds.getX() >> 8) & 0xFF;
		data[1] = bounds.getX() & 0xFF;
		data[2] = (bounds.getX2() >> 8) & 0xFF;
		data[3] = bounds.getX2() & 0xFF;
		_SPIDevice.write({ data, 4 });

		_SPIDevice.writeCommand(ROW_ADDR_SET);
		data[0] = (bounds.getY() >> 8) & 0xFF;
		data[1] = bounds.getY() & 0xFF;
		data[2] = (bounds.getY2() >> 8) & 0xFF;
		data[3] = bounds.getY2() & 0xFF;
		_SPIDevice.write({ data, 4 });

		// Memory write
		_SPIDevice.writeCommand(MEM_WR);
		_SPIDevice.write(pixelBuffer);
	}

	void GC9A01Display::turnOn() {
		_SPIDevice.writeCommand(DISPON);
		system::delayMs(20);
	}

	void GC9A01Display::turnOff() {
		_SPIDevice.writeCommand(DISPOFF);
	}
}

#endif