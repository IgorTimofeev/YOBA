#pragma once

#include <cstdint>
#include <span>

#include "SPIDisplay.h"
#include <YOBA/core/size.h>
#include <YOBA/core/rectangle.h>

namespace YOBA {
	class GC9A01Display : public SPIDisplay {
		public:
			GC9A01Display(
				uint8_t mosiPin,
				uint8_t sckPin,
				int8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency,

				const Size& size = Size(240, 240),
				Rotation rotation = Rotation::none,
				ColorModel colorModel = ColorModel::RGB565
			);

			void setup() override;
			void writeMADCTLCommand();
			void writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override;
			void turnOn();
			void turnOff();

		private:
			constexpr static uint8_t SWRESET = 0x01;   ///< Software Reset (maybe, not documented)
			constexpr static uint8_t RDDID = 0x04;     ///< Read display identification information
			constexpr static uint8_t RDDST = 0x09;     ///< Read Display Status
			constexpr static uint8_t SLPIN = 0x10;     ///< Enter Sleep Mode
			constexpr static uint8_t SLPOUT = 0x11;    ///< Sleep Out
			constexpr static uint8_t PTLON = 0x12;     ///< Partial Mode ON
			constexpr static uint8_t NORON = 0x13;     ///< Normal Display Mode ON
			constexpr static uint8_t INVOFF = 0x20;    ///< Display Inversion OFF
			constexpr static uint8_t INVON = 0x21;     ///< Display Inversion ON
			constexpr static uint8_t DISPOFF = 0x28;   ///< Display OFF
			constexpr static uint8_t DISPON = 0x29;    ///< Display ON
			constexpr static uint8_t CASET = 0x2A;     ///< Column Address Set
			constexpr static uint8_t RASET = 0x2B;     ///< Row Address Set
			constexpr static uint8_t RAMWR = 0x2C;     ///< Memory Write
			constexpr static uint8_t PTLAR = 0x30;     ///< Partial Area
			constexpr static uint8_t VSCRDEF = 0x33;   ///< Vertical Scrolling Definition
			constexpr static uint8_t TEOFF = 0x34;     ///< Tearing Effect Line OFF
			constexpr static uint8_t TEON = 0x35;      ///< Tearing Effect Line ON
			constexpr static uint8_t MADCTL = 0x36;    ///< Memory Access Control
			constexpr static uint8_t VSCRSADD = 0x37;  ///< Vertical Scrolling Start Address
			constexpr static uint8_t IDLEOFF = 0x38;   ///< Idle mode OFF
			constexpr static uint8_t IDLEON = 0x39;    ///< Idle mode ON
			constexpr static uint8_t COLMOD = 0x3A;    ///< Pixel Format Set
			constexpr static uint8_t CONTINUE = 0x3C;  ///< Write Memory Continue
			constexpr static uint8_t TEARSET = 0x44;   ///< Set Tear Scanline
			constexpr static uint8_t GETLINE = 0x45;   ///< Get Scanline
			constexpr static uint8_t SETBRIGHT = 0x51; ///< Write Display Brightness
			constexpr static uint8_t SETCTRL = 0x53;   ///< Write CTRL Display
			constexpr static uint8_t GC9A01A1_POWER7 = 0xA7;   ///< Power Control 7
			constexpr static uint8_t TEWC = 0xBA;      ///< Tearing effect width control
			constexpr static uint8_t GC9A01A1_POWER1 = 0xC1;   ///< Power Control 1
			constexpr static uint8_t GC9A01A1_POWER2 = 0xC3;   ///< Power Control 2
			constexpr static uint8_t GC9A01A1_POWER3 = 0xC4;   ///< Power Control 3
			constexpr static uint8_t GC9A01A1_POWER4 = 0xC9;   ///< Power Control 4
			constexpr static uint8_t RDID1 = 0xDA;     ///< Read ID 1
			constexpr static uint8_t RDID2 = 0xDB;     ///< Read ID 2
			constexpr static uint8_t RDID3 = 0xDC;     ///< Read ID 3
			constexpr static uint8_t FRAMERATE = 0xE8; ///< Frame rate control
			constexpr static uint8_t SPI2DATA = 0xE9;  ///< SPI 2DATA control
			constexpr static uint8_t INREGEN2 = 0xEF;  ///< Inter register enable 2
			constexpr static uint8_t GAMMA1 = 0xF0;    ///< Set gamma 1
			constexpr static uint8_t GAMMA2 = 0xF1;    ///< Set gamma 2
			constexpr static uint8_t GAMMA3 = 0xF2;    ///< Set gamma 3
			constexpr static uint8_t GAMMA4 = 0xF3;    ///< Set gamma 4
			constexpr static uint8_t IFACE = 0xF6;     ///< Interface control
			constexpr static uint8_t INREGEN1 = 0xFE;  ///< Inter register enable 1

			constexpr static uint8_t COL_ADDR_SET        = 0x2A;
			constexpr static uint8_t ROW_ADDR_SET        = 0x2B;
			constexpr static uint8_t MEM_WR              = 0x2C;
			constexpr static uint8_t COLOR_MODE          = 0x3A;
			constexpr static uint8_t COLOR_MODE__12_BIT  = 0x03;
			constexpr static uint8_t COLOR_MODE__16_BIT  = 0x05;
			constexpr static uint8_t COLOR_MODE__18_BIT  = 0x06;
			constexpr static uint8_t MEM_WR_CONT         = 0x3C;

			constexpr static uint8_t MADCTL_MY = 0x80;  ///< Bottom to top
			constexpr static uint8_t MADCTL_MX = 0x40;  ///< Right to left
			constexpr static uint8_t MADCTL_MV = 0x20;  ///< Reverse Mode
			constexpr static uint8_t MADCTL_ML = 0x10;  ///< LCD refresh Bottom to top
			constexpr static uint8_t MADCTL_RGB = 0x00; ///< Red-Green-Blue pixel order
			constexpr static uint8_t MADCTL_BGR = 0x08; ///< Blue-Green-Red pixel order
			constexpr static uint8_t MADCTL_MH = 0x04;  ///< LCD refresh right to left
	};
}