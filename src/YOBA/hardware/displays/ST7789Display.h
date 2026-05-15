#pragma once

#include <cstdint>
#include <YOBA/hardware/displays/SPIDisplay.h>
#include <YOBA/hardware/displays/invertibleDisplay.h>
#include <YOBA/main/bounds.h>

namespace YOBA {
	class ST7789Display : public SPIDisplay {
		public:
			// Recommended SPI frequencies:
			// Arduino: 40 MHz
			// ESP-IDF: 60 MHz
			ST7789Display(
				uint8_t mosiPin,
				uint8_t misoPin,
				uint8_t sckPin,
				int8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency,

				const Size& size = Size(240, 320),
				ViewportRotation rotation = ViewportRotation::clockwise0,
				ColorModel colorModel = ColorModel::RGB565
			);

			void setup() override;
			void writePixels(const Bounds& bounds, uint8_t* source, size_t length) override;

			void turnOn();
			void turnOff();

		protected:
			void onRotationChanged() override;

		private:
			void writeMADCTLCommand();

			// -------------------------------- Commands & registers --------------------------------

			// Mostly taken from https://github.com/Bodmer/TFT_eSPI/

			constexpr static uint8_t COLMOD = 0x3A;
			
			// MY, MX, MV, ML, BGR, MH, X, X 
			constexpr static uint8_t MADCTL = 0x36;
			constexpr static uint8_t MADCTL_MY = 0x80;
			constexpr static uint8_t MADCTL_MX = 0x40;
			constexpr static uint8_t MADCTL_MV = 0x20;
			constexpr static uint8_t MADCTL_ML = 0x10;
			constexpr static uint8_t MADCTL_RGB = 0x00;
			constexpr static uint8_t MADCTL_BGR = 0x08;
			constexpr static uint8_t MADCTL_MH = 0x04;
			constexpr static uint8_t MADCTL_SS  = 0x02;
			constexpr static uint8_t MADCTL_GS  = 0x01;

			constexpr static uint8_t TFT_NOP     = 0x00;
			constexpr static uint8_t TFT_SWRST   = 0x01;

			constexpr static uint8_t TFT_SLPIN   = 0x10;
			constexpr static uint8_t TFT_SLPOUT  = 0x11;
			constexpr static uint8_t TFT_NORON   = 0x13;

			constexpr static uint8_t TFT_INVOFF  = 0x20;
			constexpr static uint8_t TFT_INVON   = 0x21;
			constexpr static uint8_t TFT_DISPOFF = 0x28;
			constexpr static uint8_t TFT_DISPON  = 0x29;
			constexpr static uint8_t TFT_CASET   = 0x2A;
			constexpr static uint8_t TFT_PASET   = 0x2B;
			constexpr static uint8_t TFT_RAMWR   = 0x2C;
			constexpr static uint8_t TFT_RAMRD   = 0x2E;
			constexpr static uint8_t TFT_MADCTL  = 0x36;
			constexpr static uint8_t TFT_COLMOD  = 0x3A;

			// ST7789 specific commands used in init
			constexpr static uint8_t ST7789_NOP			= 0x00;
			constexpr static uint8_t ST7789_SWRESET		= 0x01;
			constexpr static uint8_t ST7789_RDDID		= 0x04;
			constexpr static uint8_t ST7789_RDDST		= 0x09;

			constexpr static uint8_t ST7789_RDDPM		= 0x0A;      // Read display power mode
			constexpr static uint8_t ST7789_RDD_MADCTL	= 0x0B;      // Read display MADCTL
			constexpr static uint8_t ST7789_RDD_COLMOD	= 0x0C;      // Read display pixel format
			constexpr static uint8_t ST7789_RDDIM		= 0x0D;      // Read display image mode
			constexpr static uint8_t ST7789_RDDSM		= 0x0E;      // Read display signal mode
			constexpr static uint8_t ST7789_RDDSR		= 0x0F;      // Read display self-diagnostic result (ST7789V)

			constexpr static uint8_t ST7789_SLPIN		= 0x10;
			constexpr static uint8_t ST7789_SLPOUT		= 0x11;
			constexpr static uint8_t ST7789_PTLON		= 0x12;
			constexpr static uint8_t ST7789_NORON		= 0x13;

			constexpr static uint8_t ST7789_INVOFF		= 0x20;
			constexpr static uint8_t ST7789_INVON		= 0x21;
			constexpr static uint8_t ST7789_GAMSET		= 0x26;      // Gamma set
			constexpr static uint8_t ST7789_DISPOFF		= 0x28;
			constexpr static uint8_t ST7789_DISPON		= 0x29;
			constexpr static uint8_t ST7789_CASET		= 0x2A;
			constexpr static uint8_t ST7789_RASET		= 0x2B;
			constexpr static uint8_t ST7789_RAMWR		= 0x2C;
			constexpr static uint8_t ST7789_RGBSET		= 0x2D;      // Color setting for 4096, 64K and 262K colors
			constexpr static uint8_t ST7789_RAMRD		= 0x2E;

			constexpr static uint8_t ST7789_PTLAR		= 0x30;
			constexpr static uint8_t ST7789_VSCRDEF		= 0x33;      // Vertical scrolling definition (ST7789V)
			constexpr static uint8_t ST7789_TEOFF		= 0x34;      // Tearing effect line off
			constexpr static uint8_t ST7789_TEON			= 0x35;      // Tearing effect line on
			constexpr static uint8_t ST7789_MADCTL		= 0x36;      // Memory data access control
			constexpr static uint8_t ST7789_VSCRSADD		= 0x37;      // Vertical screoll address
			constexpr static uint8_t ST7789_IDMOFF		= 0x38;      // Idle mode off
			constexpr static uint8_t ST7789_IDMON		= 0x39;      // Idle mode on
			constexpr static uint8_t ST7789_RAMWRC		= 0x3C;      // Memory write continue (ST7789V)
			constexpr static uint8_t ST7789_RAMRDC		= 0x3E;      // Memory read continue (ST7789V)
			constexpr static uint8_t ST7789_COLMOD		= 0x3A;

			constexpr static uint8_t ST7789_RAMCTRL		= 0xB0;      // RAM control
			constexpr static uint8_t ST7789_RGBCTRL		= 0xB1;      // RGB control
			constexpr static uint8_t ST7789_PORCTRL		= 0xB2;      // Porch control
			constexpr static uint8_t ST7789_FRCTRL1		= 0xB3;      // Frame rate control
			constexpr static uint8_t ST7789_PARCTRL		= 0xB5;      // Partial mode control
			constexpr static uint8_t ST7789_GCTRL		= 0xB7;      // Gate control
			constexpr static uint8_t ST7789_GTADJ		= 0xB8;      // Gate on timing adjustment
			constexpr static uint8_t ST7789_DGMEN		= 0xBA;      // Digital gamma enable
			constexpr static uint8_t ST7789_VCOMS		= 0xBB;      // VCOMS setting
			constexpr static uint8_t ST7789_LCMCTRL		= 0xC0;      // LCM control
			constexpr static uint8_t ST7789_IDSET		= 0xC1;      // ID setting
			constexpr static uint8_t ST7789_VDVVRHEN		= 0xC2;      // VDV and VRH command enable
			constexpr static uint8_t ST7789_VRHS			= 0xC3;      // VRH set
			constexpr static uint8_t ST7789_VDVSET		= 0xC4;      // VDV setting
			constexpr static uint8_t ST7789_VCMOFSET		= 0xC5;      // VCOMS offset set
			constexpr static uint8_t ST7789_FRCTR2		= 0xC6;      // FR Control 2
			constexpr static uint8_t ST7789_CABCCTRL		= 0xC7;      // CABC control
			constexpr static uint8_t ST7789_REGSEL1		= 0xC8;      // Register value section 1
			constexpr static uint8_t ST7789_REGSEL2		= 0xCA;      // Register value section 2
			constexpr static uint8_t ST7789_PWMFRSEL		= 0xCC;      // PWM frequency selection
			constexpr static uint8_t ST7789_PWCTRL1		= 0xD0;      // Power control 1
			constexpr static uint8_t ST7789_VAPVANEN		= 0xD2;      // Enable VAP/VAN signal output
			constexpr static uint8_t ST7789_CMD2EN		= 0xDF;      // Command 2 enable
			constexpr static uint8_t ST7789_PVGAMCTRL	= 0xE0;      // Positive voltage gamma control
			constexpr static uint8_t ST7789_NVGAMCTRL	= 0xE1;      // Negative voltage gamma control
			constexpr static uint8_t ST7789_DGMLUTR		= 0xE2;      // Digital gamma look-up table for red
			constexpr static uint8_t ST7789_DGMLUTB		= 0xE3;      // Digital gamma look-up table for blue
			constexpr static uint8_t ST7789_GATECTRL		= 0xE4;      // Gate control
			constexpr static uint8_t ST7789_SPI2EN		= 0xE7;      // SPI2 enable
			constexpr static uint8_t ST7789_PWCTRL2		= 0xE8;      // Power control 2
			constexpr static uint8_t ST7789_EQCTRL		= 0xE9;      // Equalize time control
			constexpr static uint8_t ST7789_PROMCTRL		= 0xEC;      // Program control
			constexpr static uint8_t ST7789_PROMEN		= 0xFA;      // Program mode enable
			constexpr static uint8_t ST7789_NVMSET		= 0xFC;      // NVM setting
			constexpr static uint8_t ST7789_PROMACT		= 0xFE;      // Program action
	};
}