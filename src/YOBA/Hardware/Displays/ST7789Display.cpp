#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstring>

#include <YOBA/Hardware/Displays/ST7789Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void ST7789Display::setup(
		SPIDisplayInterface* displayInterface,

		const Size& size,
		const Rotation rotation,
		const ColorModel colorModel
	) {
		SPIDisplay::setup(
			displayInterface,

			size,
			rotation,
			PixelOrder::YNormalXNormal,
			colorModel
		);

		// Reset pin
		if (_interface->getRSTPin() >= 0)
			_interface->toggleRSTPin(100, 100);

		uint8_t data[14];

		_interface->writeCommand(ST7789_SLPOUT);   // Sleep out
		system::delayMs(120);

		_interface->writeCommand(ST7789_NORON);    // Normal display mode on

		//------------------------------display and color format setting--------------------------------//

		writeMADCTLCommand();

		// JLX240 display datasheet
		data[0] = 0x0A;
		data[1] = 0x82;
		_interface->writeCommand(0xB6);
		_interface->write({ data, 2 });

		data[0] = 0x00;
		data[1] = 0xE0; // 5 to 6-bit conversion: r0 = r5, b0 = b5
		_interface->writeCommand(ST7789_RAMCTRL);
		_interface->write({ data, 2 });

		_interface->writeCommand(ST7789_COLMOD);
		_interface->write(0x55);

		system::delayMs(10);

		//--------------------------------ST7789V Frame rate setting----------------------------------//

		data[0] = 0x0c;
		data[1] = 0x0c;
		data[2] = 0x00;
		data[3] = 0x33;
		data[4] = 0x33;
		_interface->writeCommand(ST7789_PORCTRL);
		_interface->write({ data, 5 });

		// Voltages: VGH / VGL
		_interface->writeCommand(ST7789_GCTRL);
		_interface->write(0x35);

		//---------------------------------ST7789V Power setting--------------------------------------//

		// JLX240 display datasheet
		_interface->writeCommand(ST7789_VCOMS);
		_interface->write(0x28);

		_interface->writeCommand(ST7789_LCMCTRL);
		_interface->write(0x0C);

		data[0] = 0x01;
		data[1] = 0xFF;
		_interface->writeCommand(ST7789_VDVVRHEN);
		_interface->write({ data, 2 });

		// voltage VRHS
		_interface->writeCommand(ST7789_VRHS);
		_interface->write(0x10);

		_interface->writeCommand(ST7789_VDVSET);
		_interface->write(0x20);

		_interface->writeCommand(ST7789_FRCTR2);
		_interface->write(0x0f);

		data[0] = 0xa4;
		data[1] = 0xa1;
		_interface->writeCommand(ST7789_PWCTRL1);
		_interface->write({ data, 2 });

		//--------------------------------ST7789V gamma setting---------------------------------------//

		data[0] = 0xd0;
		data[1] = 0x00;
		data[2] = 0x02;
		data[3] = 0x07;
		data[4] = 0x0a;
		data[5] = 0x28;
		data[6] = 0x32;
		data[7] = 0x44;
		data[8] = 0x42;
		data[9] = 0x06;
		data[10] = 0x0e;
		data[11] = 0x12;
		data[12] = 0x14;
		data[13] = 0x17;
		_interface->writeCommand(ST7789_PVGAMCTRL);
		_interface->write({ data, 14 });

		data[0] = 0xd0;
		data[1] = 0x00;
		data[2] = 0x02;
		data[3] = 0x07;
		data[4] = 0x0a;
		data[5] = 0x28;
		data[6] = 0x31;
		data[7] = 0x54;
		data[8] = 0x47;
		data[9] = 0x0e;
		data[10] = 0x1c;
		data[11] = 0x17;
		data[12] = 0x1b;
		data[13] = 0x1e;
		_interface->writeCommand(ST7789_NVGAMCTRL);
		_interface->write({ data, 14 });

		_interface->writeCommand(ST7789_INVOFF);

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0xEF;
		_interface->writeCommand(ST7789_CASET);
		_interface->write({ data, 4 });

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x01;
		data[3] = 0x3F;
		_interface->writeCommand(ST7789_RASET);
		_interface->write({ data, 4 });

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		system::delayMs(120);
	}

	void ST7789Display::onRotationChanged() {
		RenderingTarget::onRotationChanged();

		writeMADCTLCommand();
	}

	void ST7789Display::writeMADCTLCommand() {
		uint8_t data = MADCTL_BGR;

		switch (getRotation()) {
			case Rotation::none:
				break;

			case Rotation::clockwise90:
				data |= MADCTL_MX | MADCTL_MY | MADCTL_MV;
				break;

			case Rotation::clockwise180:
				data |= MADCTL_MY;
				break;

			case Rotation::clockwise270:
				data |= MADCTL_MV;
				break;

			default:
				break;
		}

		_interface->writeCommand(MADCTL);
		_interface->write(data);
	}

	void ST7789Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		// Column Address Set
		data[0] = bounds.getX() >> 8; //Start Col High
		data[1] = bounds.getX() & 0xff; //Start Col Low
		data[2] = bounds.getX2() >> 8; //End Col High
		data[3] = bounds.getX2() & 0xff; //End Col Low
		_interface->writeCommand(0x2A);
		_interface->write({ data, 4});

		//Page address set
		data[0] = bounds.getY() >> 8; //Start page high
		data[1] = bounds.getY() & 0xff; // Start page low
		data[2] = bounds.getY2() >> 8; // End page high
		data[3] = bounds.getY2() & 0xff; // End page low
		_interface->writeCommand(0x2B);
		_interface->write({ data, 4});

		// Memory write
		_interface->writeCommand(0x2C);
		_interface->write(pixelBuffer);
	}

	void ST7789Display::turnOff() {
		_interface->writeCommand(ST7789_DISPOFF);
		system::delayMs(120);
	}

	void ST7789Display::turnOn() {
		_interface->writeCommand(ST7789_DISPON);
		system::delayMs(120);
	}
}

#endif