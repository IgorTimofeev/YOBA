#include "ILI9341Display.h"

namespace yoba::hardware {
	ILI9341Display::ILI9341Display(
		uint8_t mosiPin,
		uint8_t misoPin,
		uint8_t sckPin,
		uint8_t ssPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency,

		ColorModel colorModel,
		ViewportRotation rotation
	) :
		RenderTarget(
			Size(240, 320),
			PixelWriting::buffered,
			PixelOrder::YX,
			colorModel,
			rotation
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

	void ILI9341Display::setup() {
		SPIDisplay::setup();

		uint8_t b[16];

		// Power control B, power control = 0, DC_ENA = 1
		b[0] = 0x00;
		b[1] = 0x83;
		b[2] = 0x30;
		this->writeCommandAndData(0xCF, b, 3);

		// Power on sequence control,
		// cp1 keeps 1 frame, 1st frame enable
		// vcl = 0, ddvdh=3, vgh=1, vgl=2
		// DDVDH_ENH=1
		b[0] = 0x64;
		b[1] = 0x03;
		b[2] = 0x12;
		b[3] = 0x81;
		this->writeCommandAndData(0xED, b, 4);

		// Driver timing control A,
		// non-overlap=default +1
		// EQ=default - 1, CR=default
		// pre-charge=default - 1
		b[0] = 0x85;
		b[1] = 0x01;
		b[2] = 0x79;
		this->writeCommandAndData(0xE8, b, 3);

		// Power control A, Vcore=1.6V, DDVDH=5.6V
		b[0] = 0x39;
		b[1] = 0x2C;
		b[2] = 0x00;
		b[3] = 0x34;
		b[4] = 0x02;
		this->writeCommandAndData(0xCB, b, 5);

		// Pump ratio control, DDVDH=2xVCl
		this->writeCommandAndData(0xF7, 0x20);

		// Driver timing control, all=0 unit
		b[0] = 0x00;
		b[1] = 0x00;
		this->writeCommandAndData(0xEA, b, 2);

		// Power control 1, GVDD=4.75V
		this->writeCommandAndData(0xC0, 0x26);

		// Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3
		this->writeCommandAndData(0xC1, 0x11);

		// VCOM control 1, VCOMH=4.025V, VCOML=-0.950V
		b[0] = 0x35;
		b[1] = 0x3E;
		this->writeCommandAndData(0xC5, b, 2);

		// VCOM control 2, VCOMH=VMH-2, VCOML=VML-2
		this->writeCommandAndData(0xC7, 0xBE);

		// Memory access control
		writeMADCTLCommand();

		// Inversion
		setInverted(true);

		// Color model
		// 101 - 16 bits per pixel
		// 110 - 18 bits per pixel
		this->writeCommandAndData((uint8_t) Command::COLMOD, _colorModel == ColorModel::rgb666 ? 0b01100110 : 0b01010101);

		// Frame rate control, f=fosc, 70Hz fps
		b[0] = 0x00;
		b[1] = 0x1B;
		this->writeCommandAndData(0xB1, b, 2);

		// Enable 3G, disabled
		this->writeCommandAndData(0xF2, 0x08);

		// Gamma set, curve 1
		this->writeCommandAndData(0x26, 0x01);

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
		this->writeCommandAndData(0xE0, b, 16);

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
		this->writeCommandAndData(0xE1, b, 16);

		// Column address set, SC=0, EC=0xEF
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x00;
		b[3] = 0xEF;
		this->writeCommandAndData(0x2A, b, 4);

		// Page address set, SP=0, EP=0x013F
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x01;
		b[3] = 0x3f;
		this->writeCommandAndData(0x2B, b, 4);

		// Memory write
		this->writeCommand(0x2C);

		// Entry mode set, Low vol detect disabled, normal display
		this->writeCommandAndData(0xB7, 0x07);

		// Display function control
		b[0] = 0x0A;
		b[1] = 0x82;
		b[2] = 0x27;
		b[3] = 0x00;
		this->writeCommandAndData(0xB6, b, 4);

		// Sleep out
		this->writeCommandAndData(0x11, 0x00);
		system::sleep(5);

		// Clearing
		{
			memset(_buffer, 0x00, _bufferLength);

			for (uint16_t y = 0; y < _size.getHeight(); y += _bufferHeight)
				flushBuffer(Bounds(0, y, _size.getWidth(), _bufferHeight), _bufferLength);
		}

		// Display on
		this->writeCommandAndData(0x29, 0x00);
		system::sleep(100);
	}

	void ILI9341Display::onRotationChanged() {
		RenderTarget::onRotationChanged();

		writeMADCTLCommand();
	}

	uint8_t ILI9341Display::getBufferHeightForRotation() {
		return
			this->_rotation == ViewportRotation::clockwise0 || this->_rotation == ViewportRotation::clockwise180
			? 64 // 5 transactions
			: 40; // 6 transactions
	}

	void ILI9341Display::setInverted(bool value) {
		this->writeCommand(value ? 0x21 : 0x20);
	}
	
	void ILI9341Display::flushBuffer(const Bounds& bounds, size_t length) {
		uint8_t data[4];

//		ESP_LOGI("ILI", "Bounds: %ld x %ld x %d x %d", bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());

		// Column Address Set
		data[0] = bounds.getX() >> 8; //Start Col High
		data[1] = bounds.getX() & 0xff; //Start Col Low
		data[2] = (bounds.getX2()) >> 8; //End Col High
		data[3] = (bounds.getX2()) & 0xff; //End Col Low
		writeCommandAndData(0x2A, data, 4);

		//Page address set
		data[0] = bounds.getY() >> 8; //Start page high
		data[1] = bounds.getY() & 0xff; // Start page low
		data[2] = (bounds.getY2()) >> 8; // End page high
		data[3] = (bounds.getY2()) & 0xff; // End page low
		writeCommandAndData(0x2B, data, 4);

		// Memory write
		writeCommandAndData(0x2C, _buffer, length);
	}

	void ILI9341Display::writeMADCTLCommand() {
		auto data = (uint8_t) Command::MADCTL_BGR;

		switch (getRotation()) {
			case ViewportRotation::clockwise0:
				data |= (uint8_t) Command::MADCTL_MX;
				break;

			case ViewportRotation::clockwise90:
				data |= (uint8_t) Command::MADCTL_MX | (uint8_t) Command::MADCTL_MY | (uint8_t) Command::MADCTL_MV;
				break;

			case ViewportRotation::clockwise180:
				data |= (uint8_t) Command::MADCTL_MY;
				break;

			case ViewportRotation::clockwise270:
				data |= (uint8_t) Command::MADCTL_MV;
				break;

			default:
				break;
		}

		this->writeCommandAndData((uint8_t) Command::MADCTL, data);
	}

	void ILI9341Display::_fuckingPureVirtualFunction() {

	}
}