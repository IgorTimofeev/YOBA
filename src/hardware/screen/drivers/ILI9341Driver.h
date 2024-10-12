#pragma once

#include <cstdint>
#include "SPIScreenDriver.h"
#include "../../../size.h"

namespace yoba {
	template<ColorType ColorType>
	class ILI9341Driver : public SPIScreenDriver<ColorType> {
		public:
			ILI9341Driver(
				ScreenOrientation orientation,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,

				// Somehow 40 MHz works nice on Arduino SPI, buf ESP-IDF handles only 26 MHz
				// Hmm...
				uint32_t SPIFrequency = 40000000
			);

		protected:
			void writeBeginCommands() override;
			void writeOrientationChangeCommands() override;
			uint8_t getTransactionWindowHeightForOrientation() override;

		private:
			enum class Command : uint8_t {
				/* MY, MX, MV, ML, BGR, MH, X, X */
				MADCTL = 0x36,
				MADCTL_MY = 0x80,
				MADCTL_MX = 0x40,
				MADCTL_MV = 0x20,
				MADCTL_ML = 0x10,
				MADCTL_BGR = 0x08,
				MADCTL_MH = 0x04,
			};
	};

	template<ColorType ColorType>
	ILI9341Driver<ColorType>::ILI9341Driver(
		ScreenOrientation orientation,

		uint8_t csPin,
		uint8_t dcPin,
		int8_t rstPin,
		uint32_t SPIFrequency
	) :
		SPIScreenDriver<ColorType>(
			Size(240, 320),
			orientation,

			csPin,
			dcPin,
			rstPin,
			SPIFrequency
		) {

	}

	template<ColorType ColorType>
	void ILI9341Driver<ColorType>::writeOrientationChangeCommands() {
		auto data = (uint8_t) Command::MADCTL_BGR;

		switch (this->_orientation) {
			case ScreenOrientation::Portrait0:
				data |= (uint8_t) Command::MADCTL_MX;
				break;

			case ScreenOrientation::Landscape90:
				data |= (uint8_t) Command::MADCTL_MX | (uint8_t) Command::MADCTL_MY | (uint8_t) Command::MADCTL_MV;
				break;

			case ScreenOrientation::Portrait180:
				data |= (uint8_t) Command::MADCTL_MY;
				break;

			case ScreenOrientation::Landscape270:
				data |= (uint8_t) Command::MADCTL_MV;
				break;

			default:
				break;
		}

		this->writeCommandAndData((uint8_t) Command::MADCTL, data);
	}

	template<ColorType ColorType>
	uint8_t ILI9341Driver<ColorType>::getTransactionWindowHeightForOrientation() {
		return
			this->_orientation == ScreenOrientation::Portrait0 || this->_orientation == ScreenOrientation::Portrait180
			? 64 // 5 transactions
			: 40; // 6 transactions
	}

	template<ColorType ColorType>
	void ILI9341Driver<ColorType>::writeBeginCommands() {
		uint8_t b[16];

		/* Power control B, power control = 0, DC_ENA = 1 */
		b[0] = 0x00;
		b[1] = 0x83;
		b[2] = 0x30;
		this->writeCommandAndData(0xCF, b, 3);

		/* Power on sequence control,
		* cp1 keeps 1 frame, 1st frame enable
		* vcl = 0, ddvdh=3, vgh=1, vgl=2
		* DDVDH_ENH=1
		*/
		b[0] = 0x64;
		b[1] = 0x03;
		b[2] = 0x12;
		b[3] = 0x81;
		this->writeCommandAndData(0xED, b, 4);

		/* Driver timing control A,
		* non-overlap=default +1
		* EQ=default - 1, CR=default
		* pre-charge=default - 1
		*/
		b[0] = 0x85;
		b[1] = 0x01;
		b[2] = 0x79;
		this->writeCommandAndData(0xE8, b, 3);

		/* Power control A, Vcore=1.6V, DDVDH=5.6V */
		b[0] = 0x39;
		b[1] = 0x2C;
		b[2] = 0x00;
		b[3] = 0x34;
		b[4] = 0x02;
		this->writeCommandAndData(0xCB, b, 5);

		/* Pump ratio control, DDVDH=2xVCl */
		this->writeCommandAndData(0xF7, 0x20);

		/* Driver timing control, all=0 unit */
		b[0] = 0x00;
		b[1] = 0x00;
		this->writeCommandAndData(0xEA, b, 2);

		/* Power control 1, GVDD=4.75V */
		this->writeCommandAndData(0xC0, 0x26);

		/* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
		this->writeCommandAndData(0xC1, 0x11);

		/* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
		b[0] = 0x35;
		b[1] = 0x3E;
		this->writeCommandAndData(0xC5, b, 2);

		/* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
		this->writeCommandAndData(0xC7, 0xBE);

		/* Memory access control */
		writeOrientationChangeCommands();

		/* Inversion */
		this->writeCommandAndData(0x21, 0x01);

		/*
		* Pixel format for RGB/MCU interface
		* 101 - 16 bits per pixel
		* 110 - 18 bits per pixel
		*/
		this->writeCommandAndData(0x3A, 0b01010101);

		/* Frame rate control, f=fosc, 70Hz fps */
		b[0] = 0x00;
		b[1] = 0x1B;
		this->writeCommandAndData(0xB1, b, 2);

		/* Enable 3G, disabled */
		this->writeCommandAndData(0xF2, 0x08);

		/* Gamma set, curve 1 */
		this->writeCommandAndData(0x26, 0x01);

		/* Positive gamma correction */
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
		this->writeCommandAndData(0xE0, b, 15);

		/* Negative gamma correction */
		b[0] =  0x00;
		b[1] =  0x15;
		b[2] =  0x17;
		b[3] =  0x07;
		b[4] =  0x11;
		b[5] =  0x06;
		b[6] =  0x2B;
		b[7] =  0x56;
		b[8] =  0x3C;
		b[9] =  0x05;
		b[10] = 0x10;
		b[11] = 0x0F;
		b[12] = 0x3F;
		b[13] = 0x3F;
		b[15] = 0x0F;
		this->writeCommandAndData(0xE1, b, 15);

		/* Column address set, SC=0, EC=0xEF */
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x00;
		b[3] = 0xEF;
		this->writeCommandAndData(0x2A, b, 4);

		/* Page address set, SP=0, EP=0x013F */
		b[0] = 0x00;
		b[1] = 0x00;
		b[2] = 0x01;
		b[3] = 0x3f;
		this->writeCommandAndData(0x2B, b, 4);

		/* Memory write */
		this->writeCommand(0x2C);

		/* Entry mode set, Low vol detect disabled, normal display */
		b[0] = 0x07;
		this->writeCommandAndData(0xB7, b, 1);

		/* Display function control */
		b[0] = 0x0A;
		b[1] = 0x82;
		b[2] = 0x27;
		b[3] = 0x00;
		this->writeCommandAndData(0xB6, b, 4);

		/* Sleep out */
		this->writeCommandAndData(0x11, 0x00);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		/* Display on */
		this->writeCommandAndData(0x29, 0x00);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	using Rgb565ILI9341Driver = ILI9341Driver<ColorType::Rgb565>;
}