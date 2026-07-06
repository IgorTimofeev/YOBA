#include "ILI9341Display.h"
#include <YOBA/system.h>
#include <cstring>

namespace YOBA {
	void ILI9341Display::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint8_t DCPin,
		const int8_t RSTPin,
		const uint32_t SPIFrequencyHz,

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
			SPIFrequencyHz,

			size,
			rotation,
			PixelOrder::YNormalXNormal,
			colorModel
		);

		uint8_t b[16];

		// Power control B, power control = 0, DC_ENA = 1
		b[0] = 0x00;
		b[1] = 0x83;
		b[2] = 0x30;
		this->writeCommand(0xCF);
		this->writeData({ b, 3 });

		// Power on sequence control,
		// cp1 keeps 1 frame, 1st frame enable
		// vcl = 0, ddvdh=3, vgh=1, vgl=2
		// DDVDH_ENH=1
		b[0] = 0x64;
		b[1] = 0x03;
		b[2] = 0x12;
		b[3] = 0x81;
		this->writeCommand(0xED);
		this->writeData({ b, 4 });

		// Driver timing control A,
		// non-overlap=default +1
		// EQ=default - 1, CR=default
		// pre-charge=default - 1
		b[0] = 0x85;
		b[1] = 0x01;
		b[2] = 0x79;
		this->writeCommand(0xE8);
		this->writeData({ b, 3 });

		// Power control A, Vcore=1.6V, DDVDH=5.6V
		b[0] = 0x39;
		b[1] = 0x2C;
		b[2] = 0x00;
		b[3] = 0x34;
		b[4] = 0x02;
		this->writeCommand(0xCB);
		this->writeData({ b, 5 });

		// Pump ratio control, DDVDH=2xVCl
		this->writeCommand(0xF7);
		this->writeData(0x20);

		// Driver timing control, all=0 unit
		b[0] = 0x00;
		b[1] = 0x00;
		this->writeCommand(0xEA);
		this->writeData({ b, 2 });

		// Power control 1, GVDD=4.75V
		this->writeCommand(0xC0);
		this->writeData(0x26);

		// Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3
		this->writeCommand(0xC1);
		this->writeData(0x11);

		// VCOM control 1, VCOMH=4.025V, VCOML=-0.950V
		b[0] = 0x35;
		b[1] = 0x3E;
		this->writeCommand(0xC5);
		this->writeData({ b, 2 });

		// VCOM control 2, VCOMH=VMH-2, VCOML=VML-2
		this->writeCommand(0xC7);
		this->writeData(0xBE);

		// Memory access control
		writeMADCTLCommand();

		// Inversion
		setInverted(true);

		// Color model
		// 101 - 16 bits per pixel
		// 110 - 18 bits per pixel
		this->writeCommand(COLMOD);
		this->writeData(getColorModel() == ColorModel::RGB666 ? 0b01100110 : 0b01010101);

		// Frame rate control, f=fosc, 70Hz fps
		b[0] = 0x00;
		b[1] = 0x1B;
		this->writeCommand(0xB1);
		this->writeData({ b, 2 });

		// Enable 3G, disabled
		this->writeCommand(0xF2);
		this->writeData(0x08);

		// Gamma set, curve 1
		this->writeCommand(0x26);
		this->writeData(0x01);

		// Positive gamma correction
		b[0] = 0x0F;
		b[1] = 0x2A;
		b[2] = 0x28;
		b[3] = 0x08;
		b[4] = 0x0E;
		b[5] = 0x08;
		b[6] = 0x54;
		b[7] = 0xA9;
		b[8] = 0x43;
		b[9] = 0x0A;
		b[10] = 0x0F;
		b[11] = 0x00;
		b[12] = 0x00;
		b[13] = 0x00;
		b[15] = 0x00;
		this->writeCommand(0xE0);
		this->writeData({ b, 16 });

		// Negative gamma correction
		b[0] = 0x00;
		b[1] = 0x15;
		b[2] = 0x17;
		b[3] = 0x07;
		b[4] = 0x11;
		b[5] = 0x06;
		b[6] = 0x2B;
		b[7] = 0x56;
		b[8] = 0x3C;
		b[9] = 0x05;
		b[10] = 0x10;
		b[11] = 0x0F;
		b[12] = 0x3F;
		b[13] = 0x3F;
		b[15] = 0x0F;
		this->writeCommand(0xE1);
		this->writeData({ b, 16 });

		// Column address set, SC=0, EC=0xEF
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x00;
		b[3] = 0xEF;
		this->writeCommand(0x2A);
		this->writeData({ b, 4 });

		// Page address set, SP=0, EP=0x013F
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x01;
		b[3] = 0x3f;
		this->writeCommand(0x2B);
		this->writeData({ b, 4 });

		// Memory write
		this->writeCommand(0x2C);

		// Entry mode set, Low vol detect disabled, normal display
		this->writeCommand(0xB7);
		this->writeData(0x07);

		// Display function control
		b[0] = 0x0A;
		b[1] = 0x82;
		b[2] = 0x27;
		b[3] = 0x00;
		this->writeCommand(0xB6);
		this->writeData({ b, 4 });

		// Sleep out
		this->writeCommand(0x11);
		this->writeData(0x00);

		system::delayMs(5);
	}

	void ILI9341Display::onRotationChanged() {
		RenderingTarget::onRotationChanged();

		writeMADCTLCommand();
	}

	void ILI9341Display::setInverted(const bool value) {
		this->writeCommand(value ? 0x21 : 0x20);
	}

	void ILI9341Display::writeMADCTLCommand() {
		// LMAO seems like ILI9341 has reverse color order by default for some reason
		auto data = MADCTL_BGR;

		switch (getRotation()) {
			case Rotation::none:
				data |= MADCTL_MX;
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

	void ILI9341Display::writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		uint8_t data[4];

//		ESP_LOGI("ILI", "Bounds: %ld x %ld x %d x %d", bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());

		// Column Address Set
		data[0] = bounds.getX() >> 8; //Start Col High
		data[1] = bounds.getX() & 0xff; //Start Col Low
		data[2] = bounds.getX2() >> 8; //End Col High
		data[3] = bounds.getX2() & 0xff; //End Col Low
		this->writeCommand(0x2A);
		this->writeData({ data, 4 });

		//Page address set
		data[0] = bounds.getY() >> 8; //Start page high
		data[1] = bounds.getY() & 0xff; // Start page low
		data[2] = bounds.getY2() >> 8; // End page high
		data[3] = bounds.getY2() & 0xff; // End page low
		this->writeCommand(0x2B);
		this->writeData({ data, 4 });

		// Memory write
		this->writeCommand(0x2C);
		this->writeData(pixelBuffer);
	}

	void ILI9341Display::turnOff() {
		this->writeCommand(0x28);
	}

	void ILI9341Display::turnOn() {
		this->writeCommand(0x29);
	}
}