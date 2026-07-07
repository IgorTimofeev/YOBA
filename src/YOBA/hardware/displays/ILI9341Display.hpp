#pragma once

#include <cstdint>
#include <span>

#include <YOBA/Hardware/Displays/SPIDisplay.hpp>
#include <YOBA/Hardware/Displays/InvertibleDisplay.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	class ILI9341Display : public SPIDisplay, public InvertibleDisplay {
		public:
			void setup(
				uint8_t MOSIPin,
				uint8_t SCKPin,
				int8_t SSPin,
				uint8_t DCPin,
				int8_t RSTPin,
				uint32_t SPIFrequencyHz = 60'000'000,

				const Size& size = Size(240, 320),
				Rotation rotation = Rotation::none,
				ColorModel colorModel = ColorModel::RGB565
			);

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