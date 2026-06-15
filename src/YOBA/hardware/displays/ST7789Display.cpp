#include "ST7789Display.h"
#include <YOBA/system.h>
#include <cstring>

namespace YOBA {
	ST7789Display::ST7789Display(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequency,

		const Size& size,
		const Rotation rotation,
		const ColorModel colorModel
	) :
		RenderingTarget(
			size,
			rotation,
			PixelOrder::YX,
			colorModel
		),
		SPIDisplay(
			MOSIPin,
			SCKPin,
			SSPin,
			DCPin,
			RSTPin,
			SPIFrequency
		)
	{

	}

	void ST7789Display::setup() {
		SPIDisplay::setup();

		uint8_t data[14];

		this->writeCommand(ST7789_SLPOUT);   // Sleep out
		system::delayMs(120);

		this->writeCommand(ST7789_NORON);    // Normal display mode on

		//------------------------------display and color format setting--------------------------------//

		writeMADCTLCommand();

		// JLX240 display datasheet
		data[0] = 0x0A;
		data[1] = 0x82;
		this->writeCommand(0xB6);
		this->writeData({ data, 2 });

		data[0] = 0x00;
		data[1] = 0xE0; // 5 to 6-bit conversion: r0 = r5, b0 = b5
		this->writeCommand(ST7789_RAMCTRL);
		this->writeData({ data, 2 });

		this->writeCommand(ST7789_COLMOD);
		this->writeData(0x55);

		system::delayMs(10);

		//--------------------------------ST7789V Frame rate setting----------------------------------//

		data[0] = 0x0c;
		data[1] = 0x0c;
		data[2] = 0x00;
		data[3] = 0x33;
		data[4] = 0x33;
		this->writeCommand(ST7789_PORCTRL);
		this->writeData({ data, 5 });

		// Voltages: VGH / VGL
		this->writeCommand(ST7789_GCTRL);
		this->writeData(0x35);

		//---------------------------------ST7789V Power setting--------------------------------------//

		// JLX240 display datasheet
		this->writeCommand(ST7789_VCOMS);
		this->writeData(0x28);

		this->writeCommand(ST7789_LCMCTRL);
		this->writeData(0x0C);

		data[0] = 0x01;
		data[1] = 0xFF;
		this->writeCommand(ST7789_VDVVRHEN);
		this->writeData({ data, 2 });

		// voltage VRHS
		this->writeCommand(ST7789_VRHS);
		this->writeData(0x10);

		this->writeCommand(ST7789_VDVSET);
		this->writeData(0x20);

		this->writeCommand(ST7789_FRCTR2);
		this->writeData(0x0f);

		data[0] = 0xa4;
		data[1] = 0xa1;
		this->writeCommand(ST7789_PWCTRL1);
		this->writeData({ data, 2 });

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
		this->writeCommand(ST7789_PVGAMCTRL);
		this->writeData({ data, 14 });

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
		this->writeCommand(ST7789_NVGAMCTRL);
		this->writeData({ data, 14 });

		this->writeCommand(ST7789_INVOFF);

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0xEF;
		this->writeCommand(ST7789_CASET);
		this->writeData({ data, 4 });

		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x01;
		data[3] = 0x3F;
		this->writeCommand(ST7789_RASET);
		this->writeData({ data, 4 });

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

		this->writeCommand(MADCTL);
		this->writeData(data);
	}

	void ST7789Display::writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

		// Column Address Set
		data[0] = bounds.getX() >> 8; //Start Col High
		data[1] = bounds.getX() & 0xff; //Start Col Low
		data[2] = bounds.getX2() >> 8; //End Col High
		data[3] = bounds.getX2() & 0xff; //End Col Low
		this->writeCommand(0x2A);
		this->writeData({ data, 4});

		//Page address set
		data[0] = bounds.getY() >> 8; //Start page high
		data[1] = bounds.getY() & 0xff; // Start page low
		data[2] = bounds.getY2() >> 8; // End page high
		data[3] = bounds.getY2() & 0xff; // End page low
		this->writeCommand(0x2B);
		this->writeData({ data, 4});

		// Memory write
		this->writeCommand(0x2C);
		this->writeData(pixelBuffer);
	}

	void ST7789Display::turnOff() {
		this->writeCommand(ST7789_DISPOFF);
		system::delayMs(120);
	}

	void ST7789Display::turnOn() {
		this->writeCommand(ST7789_DISPON);
		system::delayMs(120);
	}
}