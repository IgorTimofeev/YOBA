#pragma once

#include <cstdint>
#include "../../rendering/targets/bufferedRenderTarget.h"
#include "SPIDisplay.h"

namespace yoba::hardware {
	class ILI9341Display : public SPIDisplay, public BufferedRenderTarget {
		public:
			ILI9341Display(
				ColorModel colorModel,
				RenderTargetOrientation orientation,

				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,

				// Somehow 40 MHz works nice on Arduino SPI, buf ESP-IDF handles only 26 MHz
				// Hmm...
				uint32_t SPIFrequency = 40000000
			);

		protected:
			void writeSetupCommands() override;
			void writeOrientationChangeCommands() override;
			void writeColorModeChangeCommands() override;

			uint8_t getBufferHeightForOrientation() override;

		public:
			void flushBuffer(uint16_t y) override;

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
	};
}