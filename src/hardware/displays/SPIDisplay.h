#pragma once

#include "../../system.h"
#include <cstdint>
#include "../../rendering/targets/renderTarget.h"

namespace yoba::hardware {
	class SPIDisplay : public virtual RenderTarget {
		public:
			SPIDisplay(
				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency
			);

			void setup() override;

		protected:
			void onOrientationChanged() override;

			void setChipSelect(uint8_t value) const;
			void setDataCommand(uint8_t value) const;

			void writeData(uint8_t data);
			void writeData(const uint8_t *data, size_t length);

			void writeCommand(uint8_t data);

			void writeCommandAndData(uint8_t command, const uint8_t *data, size_t length);
			void writeCommandAndData(uint8_t command, uint8_t data);

			virtual void writeSetupCommands() = 0;
			virtual void writeOrientationChangeCommands() = 0;
			virtual void writeColorModeChangeCommands() = 0;

		private:
			uint8_t _csPin;
			uint8_t _dcPin;
			int8_t _rstPin;
			uint32_t _SPIFrequency;
	};
}