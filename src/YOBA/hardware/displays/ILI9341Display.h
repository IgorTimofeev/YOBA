#pragma once

#include <cstdint>
#include "SPIDisplay.h"
#include "YOBA/hardware/displays/traits/invertibleDisplay.h"
#include "YOBA/main/rendering/bufferedRenderTarget.h"

namespace YOBA {
	class ILI9341Display : public SPIDisplay, public InvertibleDisplay, public BufferedRenderTarget {
		public:
			// Recommended SPI frequencies:
			// Arduino: 40 MHz
			// ESP-IDF: 26 MHz
			ILI9341Display(
				uint8_t mosiPin,
				uint8_t misoPin,
				uint8_t sckPin,
				uint8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency,

				const Size& size = Size(240, 320),
				ColorModel colorModel = ColorModel::rgb565,
				ViewportRotation rotation = ViewportRotation::clockwise0
			);

			void setup() override;
			void setInverted(bool value) override;
			void flushBuffer(const Bounds& bounds, size_t length) override;

		protected:
			uint8_t getBufferHeightForRotation() override;
			void onRotationChanged() override;

		private:
			enum class Command : uint8_t {
				COLMOD = 0x3A,

				/* MY, MX, MV, ML, BGR, MH, X, X */
				MADCTL = 0x36,
				MADCTL_MY = 0x80,
				MADCTL_MX = 0x40,
				MADCTL_MV = 0x20,
				MADCTL_ML = 0x10,
				MADCTL_BGR = 0x08,
				MADCTL_MH = 0x04,
			};

			void writeMADCTLCommand();
	};
}