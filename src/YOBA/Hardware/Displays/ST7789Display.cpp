#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <cstring>

#include <YOBA/Hardware/Displays/ST7789Display.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void ST7789Display::setup(
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
			PixelOrder::YNormalXNormal,
			colorModel
		);

		uint8_t data[14];

		_SPIDevice.writeCommand(ST7789_SLPOUT);   // Sleep out
		system::delayMs(120);

		_SPIDevice.writeCommand(ST7789_NORON);    // Normal display mode on

		//------------------------------display and color format setting--------------------------------//

		writeMADCTLCommand();

		// JLX240 display datasheet
		data[0] = 0x0A;
		data[1] = 0x82;
		_SPIDevice.writeCommand(0xB6);
		_SPIDevice.write({ data, 2 });

		data[0] = 0x00;
		data[1] = 0xE0; // 5 to 6-bit conversion: r0 = r5, b0 = b5
		_SPIDevice.writeCommand(ST7789_RAMCTRL);
		_SPIDevice.write({ data, 2 });

		_SPIDevice.writeCommand(ST7789_COLMOD);
		_SPIDevice.write(0x55);

		system::delayMs(10);

		//--------------------------------ST7789V Frame rate setting----------------------------------//

		data[0] = 0x0c;
		data[1] = 0x0c;
		data[2] = 0x00;
		data[3] = 0x33;
		data[4] = 0x33;
		_SPIDevice.writeCommand(ST7789_PORCTRL);
		_SPIDevice.write({ data, 5 });

		// Voltages: VGH / VGL
		_SPIDevice.writeCommand(ST7789_GCTRL);
		_SPIDevice.write(0x35);

		//---------------------------------ST7789V Power setting--------------------------------------//

		// JLX240 display datasheet
		_SPIDevice.writeCommand(ST7789_VCOMS);
		_SPIDevice.write(0x28);

		_SPIDevice.writeCommand(ST7789_LCMCTRL);
		_SPIDevice.write(0x0C);

		data[0] = 0x01;
		data[1] = 0xFF;
		_SPIDevice.writeCommand(ST7789_VDVVRHEN);
		_SPIDevice.write({ data, 2 });

		// voltage VRHS
		_SPIDevice.writeCommand(ST7789_VRHS);
		_SPIDevice.write(0x10);

		_SPIDevice.writeCommand(ST7789_VDVSET);
		_SPIDevice.write(0x20);

		_SPIDevice.writeCommand(ST7789_FRCTR2);
		_SPIDevice.write(0x0f);

		data[0] = 0xa4;
		data[1] = 0xa1;
		_SPIDevice.writeCommand(ST7789_PWCTRL1);
		_SPIDevice.write({ data, 2 });

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
		_SPIDevice.writeCommand(ST7789_PVGAMCTRL);
		_SPIDevice.write({ data, 14 });

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
		_SPIDevice.writeCommand(ST7789_NVGAMCTRL);
		_SPIDevice.write({ data, 14 });

		_SPIDevice.writeCommand(ST7789_INVOFF);

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0xEF;
		_SPIDevice.writeCommand(ST7789_CASET);
		_SPIDevice.write({ data, 4 });

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x01;
		data[3] = 0x3F;
		_SPIDevice.writeCommand(ST7789_RASET);
		_SPIDevice.write({ data, 4 });

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

		_SPIDevice.writeCommand(MADCTL);
		_SPIDevice.write(data);
	}

	void ST7789Display::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		// Column Address Set
		data[0] = bounds.getX() >> 8; //Start Col High
		data[1] = bounds.getX() & 0xff; //Start Col Low
		data[2] = bounds.getX2() >> 8; //End Col High
		data[3] = bounds.getX2() & 0xff; //End Col Low
		_SPIDevice.writeCommand(0x2A);
		_SPIDevice.write({ data, 4});

		//Page address set
		data[0] = bounds.getY() >> 8; //Start page high
		data[1] = bounds.getY() & 0xff; // Start page low
		data[2] = bounds.getY2() >> 8; // End page high
		data[3] = bounds.getY2() & 0xff; // End page low
		_SPIDevice.writeCommand(0x2B);
		_SPIDevice.write({ data, 4});

		// Memory write
		_SPIDevice.writeCommand(0x2C);
		_SPIDevice.write(pixelBuffer);
	}

	void ST7789Display::turnOff() {
		_SPIDevice.writeCommand(ST7789_DISPOFF);
		system::delayMs(120);
	}

	void ST7789Display::turnOn() {
		_SPIDevice.writeCommand(ST7789_DISPON);
		system::delayMs(120);
	}
}

#endif