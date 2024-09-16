#include <cstdint>
#include "ILI9341Driver.h"

namespace yoba {
	ILI9341Driver::ILI9341Driver(
		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		ScreenOrientation orientation
	) :
	ScreenDriver(
		csPin,
		dcPin,
		rstPin,
		Size(240, 320),
		orientation
	) {
		// Glitches & tearing can appear on 26m, 40m+ won't work anyway
		setSPIFrequency(SPI_MASTER_FREQ_26M);
	}

	void ILI9341Driver::setOrientation(ScreenOrientation orientation) {
		ScreenDriver::setOrientation(orientation);

		sendMemoryAccessControl();
	}

	uint8_t ILI9341Driver::getTransactionBufferHeightForOrientation() {
		return
			getOrientation() == ScreenOrientation::Portrait0 || getOrientation() == ScreenOrientation::Portrait180
			? 64 // 5 transactions
			: 40; // 6 transactions
	}

	void ILI9341Driver::writeInitializationCommands() {
		uint8_t b[16];

		/* Power control B, power control = 0, DC_ENA = 1 */
		b[0] = 0x00;
		b[1] = 0x83;
		b[2] = 0x30;
		sendCommandAndData(0xCF, b, 3);

		/* Power on sequence control,
		* cp1 keeps 1 frame, 1st frame enable
		* vcl = 0, ddvdh=3, vgh=1, vgl=2
		* DDVDH_ENH=1
		*/
		b[0] = 0x64;
		b[1] = 0x03;
		b[2] = 0x12;
		b[3] = 0x81;
		sendCommandAndData(0xED, b, 4);

		/* Driver timing control A,
		* non-overlap=default +1
		* EQ=default - 1, CR=default
		* pre-charge=default - 1
		*/
		b[0] = 0x85;
		b[1] = 0x01;
		b[2] = 0x79;
		sendCommandAndData(0xE8, b, 3);

		/* Power control A, Vcore=1.6V, DDVDH=5.6V */
		b[0] = 0x39;
		b[1] = 0x2C;
		b[2] = 0x00;
		b[3] = 0x34;
		b[4] = 0x02;
		sendCommandAndData(0xCB, b, 5);

		/* Pump ratio control, DDVDH=2xVCl */
		b[0] = 0x20;
		sendCommandAndData(0xF7, b, 1);

		/* Driver timing control, all=0 unit */
		b[0] = 0x00;
		b[1] = 0x00;
		sendCommandAndData(0xEA, b, 2);

		/* Power control 1, GVDD=4.75V */
		b[0] = 0x26;
		sendCommandAndData(0xC0, b, 1);

		/* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
		b[0] = 0x11;
		sendCommandAndData(0xC1, b, 1);

		/* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
		b[0] = 0x35;
		b[1] = 0x3E;
		sendCommandAndData(0xC5, b, 2);

		/* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
		b[0] = 0xBE;
		sendCommandAndData(0xC7, b, 1);

		/* Memory access control */
		sendMemoryAccessControl();

		/* Inversion */
		b[0] = 0x01;
		sendCommandAndData(0x21, b, 1);

		/* Pixel format, 16bits/pixel for RGB/MCU interface */
		b[0] = 0x55;
		sendCommandAndData(0x3A, b, 1);

		/* Frame rate control, f=fosc, 70Hz fps */
		b[0] = 0x00;
		b[1] = 0x1B;
		sendCommandAndData(0xB1, b, 2);

		/* Enable 3G, disabled */
		b[0] = 0x08;
		sendCommandAndData(0xF2, b, 1);

		/* Gamma set, curve 1 */
		b[0] = 0x01;
		sendCommandAndData(0x26, b, 1);

		/* Positive gamma correction */
		b[0] = 0x1F;
		b[1] = 0x1A;
		b[2] = 0x18;
		b[3] = 0x0A;
		b[4] = 0x0F;
		b[5] = 0x06;
		b[6] = 0x45;
		b[7] = 0x87;
		b[8] = 0x32;
		b[9] = 0x0A;
		b[10] = 0x07;
		b[11] = 0x02;
		b[12] = 0x07;
		b[13] = 0x05;
		b[15] = 0x00;
		sendCommandAndData(0xE0, b, 15);

		/* Negative gamma correction */
		b[0] = 0x00;
		b[1] = 0x25;
		b[2] = 0x27;
		b[3] = 0x05;
		b[4] = 0x10;
		b[5] = 0x09;
		b[6] = 0x3A;
		b[7] = 0x78;
		b[8] = 0x4D;
		b[9] = 0x05;
		b[10] = 0x18;
		b[11] = 0x0D;
		b[12] = 0x38;
		b[13] = 0x3A;
		b[15] = 0x1F;
		sendCommandAndData(0xE1, b, 15);

		/* Column address set, SC=0, EC=0xEF */
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x00;
		b[3] = 0xEF;
		sendCommandAndData(0x2A, b, 4);

		/* Page address set, SP=0, EP=0x013F */
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x01;
		b[3] = 0x3f;
		sendCommandAndData(0x2B, b, 4);

		/* Memory write */
		b[0] = 0x00;
		sendCommandAndData(0x2C, b, 0);

		/* Entry mode set, Low vol detect disabled, normal display */
		b[0] = 0x07;
		sendCommandAndData(0xB7, b, 1);

		/* Display function control */
		b[0] = 0x0A;
		b[1] = 0x82;
		b[2] = 0x27;
		b[3] = 0x00;
		sendCommandAndData(0xB6, b, 4);

		/* Sleep out */
		b[0] = 0x00;
		sendCommandAndData(0x11, b, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		/* Display on */
		b[0] = 0x00;
		sendCommandAndData(0x29, b, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	void ILI9341Driver::sendMemoryAccessControl() {
		auto data = (uint8_t) Command::MADCTL_BGR;

		switch (_orientation) {
			case ScreenOrientation::Portrait0:
				data |= (uint8_t) Command::MADCTL_MX;
				break;

			case ScreenOrientation::Landscape90:
				data |= (uint8_t) Command::MADCTL_MV;
				break;

			case ScreenOrientation::Portrait180:
				data |= (uint8_t) Command::MADCTL_MY;
				break;

			case ScreenOrientation::Landscape270:
				data |= (uint8_t) Command::MADCTL_MX | (uint8_t) Command::MADCTL_MY | (uint8_t) Command::MADCTL_MV;
				break;

			default:
				break;
		}

		sendCommandAndData((uint8_t) Command::MADCTL, data);
	}
}