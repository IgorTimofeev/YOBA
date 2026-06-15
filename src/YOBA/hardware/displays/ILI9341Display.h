#pragma once

#include <cstdint>
#include <span>

#include <YOBA/hardware/displays/SPIDisplay.h>
#include <YOBA/hardware/displays/invertibleDisplay.h>
#include <YOBA/core/rectangle.h>

namespace YOBA {
	class ILI9341Display : public SPIDisplay, public InvertibleDisplay {
		public:
			// Recommended SPI frequencies:
			// Arduino: 40 MHz
			// ESP-IDF: 60 MHz
			ILI9341Display(
				uint8_t mosiPin,
				uint8_t sckPin,
				int8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency,

				const Size& size = Size(240, 320),
				Rotation rotation = Rotation::none,
				ColorModel colorModel = ColorModel::RGB565
			);

			void setup() override;
			void setInverted(bool value) override;
			void writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override;

			void turnOn();
			void turnOff();

		protected:
			void onRotationChanged() override;

		private:
			constexpr static uint8_t COLMOD = 0x3A;

			/* MY, MX, MV, ML, BGR, MH, X, X */
			constexpr static uint8_t MADCTL = 0x36;
			constexpr static uint8_t MADCTL_MY = 0x80;
			constexpr static uint8_t MADCTL_MX = 0x40;
			constexpr static uint8_t MADCTL_MV = 0x20;
			constexpr static uint8_t MADCTL_ML = 0x10;
			constexpr static uint8_t MADCTL_BGR = 0x08;
			constexpr static uint8_t MADCTL_MH = 0x04;

			void writeMADCTLCommand();
	};
}