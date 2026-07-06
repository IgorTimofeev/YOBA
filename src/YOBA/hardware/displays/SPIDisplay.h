#pragma once

#include <cstdint>
#include <span>

#include <YOBA/hardware/displays/display.h>
#include <YOBA/system.h>

namespace YOBA {
	class SPIDisplay : public virtual Display {
		public:
			~SPIDisplay() override = default;

			void setup(
				const uint8_t MOSIPin,
				const uint8_t SCKPin,
				const int8_t SSPin,
				const uint8_t DCPin,
				const int8_t RSTPin,
				const uint32_t SPIFrequencyHz,

				const Size& size,
				const Rotation rotation,
				const PixelOrder pixelOrder,
				const ColorModel colorModel
			);

		protected:
			system::SPIDevice _SPIDevice {};
			int8_t _RSTPin = 0;

			void setResetPin(bool value) const;

			void writeCommand(uint8_t command);
			void writeData(uint8_t data);
			void writeData(const std::span<uint8_t> data);

			virtual void toggleResetPin();

		private:
			// No one should call this anymore
			using RenderingTarget::setup;
	};
}