#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstdint>

#include <YOBA/Hardware/Displays/GC9A01Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void GC9A01Display::setup(
		SPIDisplayInterface* displayInterface,

		const Size& size,
		const Rotation rotation,
		const ColorModel colorModel
	) {
		SPIDisplay::setup(
			displayInterface,

			size,
			rotation,
			PixelOrder::XNormalYNormal,
			colorModel
		);

		// GPIO reset
		if (_interface->getRSTPin() >= 0) {
			_interface->toggleRSTPin(150, 150);
		}
		// Software reset
		else {
			_interface->writeCommand(SWRESET);
			system::delayMs(150);
		}

	    _interface->writeCommand(0xEF);

	    _interface->writeCommand(0xEB);
	    _interface->write(0x14);

	    _interface->writeCommand(0xFE);
	    _interface->writeCommand(0xEF);

	    _interface->writeCommand(0xEB);
	    _interface->write(0x14);

	    _interface->writeCommand(0x84);
	    _interface->write(0x40);

	    _interface->writeCommand(0x85);
	    _interface->write(0xFF);

	    _interface->writeCommand(0x86);
	    _interface->write(0xFF);

	    _interface->writeCommand(0x87);
	    _interface->write(0xFF);

	    _interface->writeCommand(0x88);
	    _interface->write(0x0A);

	    _interface->writeCommand(0x89);
	    _interface->write(0x21);

	    _interface->writeCommand(0x8A);
	    _interface->write(0x00);

	    _interface->writeCommand(0x8B);
	    _interface->write(0x80);

	    _interface->writeCommand(0x8C);
	    _interface->write(0x01);

	    _interface->writeCommand(0x8D);
	    _interface->write(0x01);

	    _interface->writeCommand(0x8E);
	    _interface->write(0xFF);

	    _interface->writeCommand(0x8F);
	    _interface->write(0xFF);

	    _interface->writeCommand(0xB6);
	    _interface->write(0x00);
	    _interface->write(0x00);

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

	    _interface->writeCommand(COLOR_MODE);
	    _interface->write(getColorModel() == ColorModel::RGB565 ? COLOR_MODE__16_BIT : COLOR_MODE__18_BIT);

	    _interface->writeCommand(0x90);
	    _interface->write(0x08);
	    _interface->write(0x08);
	    _interface->write(0x08);
	    _interface->write(0x08);

	    _interface->writeCommand(0xBD);
	    _interface->write(0x06);

	    _interface->writeCommand(0xBC);
	    _interface->write(0x00);

	    _interface->writeCommand(0xFF);
	    _interface->write(0x60);
	    _interface->write(0x01);
	    _interface->write(0x04);

	    _interface->writeCommand(0xC3);
	    _interface->write(0x13);
	    _interface->writeCommand(0xC4);
	    _interface->write(0x13);

	    _interface->writeCommand(0xC9);
	    _interface->write(0x22);

	    _interface->writeCommand(0xBE);
	    _interface->write(0x11);

	    _interface->writeCommand(0xE1);
	    _interface->write(0x10);
	    _interface->write(0x0E);

	    _interface->writeCommand(0xDF);
	    _interface->write(0x21);
	    _interface->write(0x0c);
	    _interface->write(0x02);

	    _interface->writeCommand(0xF0);
	    _interface->write(0x45);
	    _interface->write(0x09);
	    _interface->write(0x08);
	    _interface->write(0x08);
	    _interface->write(0x26);
	    _interface->write(0x2A);

	    _interface->writeCommand(0xF1);
	    _interface->write(0x43);
	    _interface->write(0x70);
	    _interface->write(0x72);
	    _interface->write(0x36);
	    _interface->write(0x37);
	    _interface->write(0x6F);

	    _interface->writeCommand(0xF2);
	    _interface->write(0x45);
	    _interface->write(0x09);
	    _interface->write(0x08);
	    _interface->write(0x08);
	    _interface->write(0x26);
	    _interface->write(0x2A);

	    _interface->writeCommand(0xF3);
	    _interface->write(0x43);
	    _interface->write(0x70);
	    _interface->write(0x72);
	    _interface->write(0x36);
	    _interface->write(0x37);
	    _interface->write(0x6F);

	    _interface->writeCommand(0xED);
	    _interface->write(0x1B);
	    _interface->write(0x0B);

	    _interface->writeCommand(0xAE);
	    _interface->write(0x77);

	    _interface->writeCommand(0xCD);
	    _interface->write(0x63);

	    _interface->writeCommand(0x70);
	    _interface->write(0x07);
	    _interface->write(0x07);
	    _interface->write(0x04);
	    _interface->write(0x0E);
	    _interface->write(0x0F);
	    _interface->write(0x09);
	    _interface->write(0x07);
	    _interface->write(0x08);
	    _interface->write(0x03);

	    _interface->writeCommand(0xE8);
	    _interface->write(0x34);

	    _interface->writeCommand(0x62);
	    _interface->write(0x18);
	    _interface->write(0x0D);
	    _interface->write(0x71);
	    _interface->write(0xED);
	    _interface->write(0x70);
	    _interface->write(0x70);
	    _interface->write(0x18);
	    _interface->write(0x0F);
	    _interface->write(0x71);
	    _interface->write(0xEF);
	    _interface->write(0x70);
	    _interface->write(0x70);

	    _interface->writeCommand(0x63);
	    _interface->write(0x18);
	    _interface->write(0x11);
	    _interface->write(0x71);
	    _interface->write(0xF1);
	    _interface->write(0x70);
	    _interface->write(0x70);
	    _interface->write(0x18);
	    _interface->write(0x13);
	    _interface->write(0x71);
	    _interface->write(0xF3);
	    _interface->write(0x70);
	    _interface->write(0x70);

	    _interface->writeCommand(0x64);
	    _interface->write(0x28);
	    _interface->write(0x29);
	    _interface->write(0xF1);
	    _interface->write(0x01);
	    _interface->write(0xF1);
	    _interface->write(0x00);
	    _interface->write(0x07);

	    _interface->writeCommand(0x66);
	    _interface->write(0x3C);
	    _interface->write(0x00);
	    _interface->write(0xCD);
	    _interface->write(0x67);
	    _interface->write(0x45);
	    _interface->write(0x45);
	    _interface->write(0x10);
	    _interface->write(0x00);
	    _interface->write(0x00);
	    _interface->write(0x00);

	    _interface->writeCommand(0x67);
	    _interface->write(0x00);
	    _interface->write(0x3C);
	    _interface->write(0x00);
	    _interface->write(0x00);
	    _interface->write(0x00);
	    _interface->write(0x01);
	    _interface->write(0x54);
	    _interface->write(0x10);
	    _interface->write(0x32);
	    _interface->write(0x98);

	    _interface->writeCommand(0x74);
	    _interface->write(0x10);
	    _interface->write(0x85);
	    _interface->write(0x80);
	    _interface->write(0x00);
	    _interface->write(0x00);
	    _interface->write(0x4E);
	    _interface->write(0x00);

	    _interface->writeCommand(0x98);
	    _interface->write(0x3e);
	    _interface->write(0x07);

	    _interface->writeCommand(0x35);
	    _interface->writeCommand(0x21);

		_interface->writeCommand(GAMMA1);
		_interface->write(0x45);
		_interface->write(0x09);
		_interface->write(0x08);
		_interface->write(0x08);
		_interface->write(0x26);
		_interface->write(0x2a);

		_interface->writeCommand(GAMMA2);
		_interface->write(0x43);
		_interface->write(0x70);
		_interface->write(0x72);
		_interface->write(0x36);
		_interface->write(0x37);
		_interface->write(0x6f);

		_interface->writeCommand(GAMMA3);
		_interface->write(0x45);
		_interface->write(0x09);
		_interface->write(0x08);
		_interface->write(0x08);
		_interface->write(0x26);
		_interface->write(0x2a);

		_interface->writeCommand(GAMMA4);
		_interface->write(0x43);
		_interface->write(0x70);
		_interface->write(0x72);
		_interface->write(0x36);
		_interface->write(0x37);
		_interface->write(0x6f);

	    _interface->writeCommand(SLPOUT);
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

		_interface->writeCommand(MADCTL);
		_interface->write(data);
	}

	void GC9A01Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		_interface->writeCommand(COL_ADDR_SET);
		data[0] = (bounds.getX() >> 8) & 0xFF;
		data[1] = bounds.getX() & 0xFF;
		data[2] = (bounds.getX2() >> 8) & 0xFF;
		data[3] = bounds.getX2() & 0xFF;
		_interface->write({ data, 4 });

		_interface->writeCommand(ROW_ADDR_SET);
		data[0] = (bounds.getY() >> 8) & 0xFF;
		data[1] = bounds.getY() & 0xFF;
		data[2] = (bounds.getY2() >> 8) & 0xFF;
		data[3] = bounds.getY2() & 0xFF;
		_interface->write({ data, 4 });

		// Memory write
		_interface->writeCommand(MEM_WR);
		_interface->write(pixelBuffer);
	}

	void GC9A01Display::turnOn() {
		_interface->writeCommand(DISPON);
		system::delayMs(20);
	}

	void GC9A01Display::turnOff() {
		_interface->writeCommand(DISPOFF);
	}
}

#endif