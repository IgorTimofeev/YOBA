#pragma once

#include <cstdint>
#include "YOBA/main/rendering/renderTarget.h"

namespace YOBA {
	class SPIDisplay : public virtual RenderTarget {
		public:
			SPIDisplay(
				uint8_t mosiPin,
				uint8_t misoPin,
				uint8_t sckPin,
				uint8_t ssPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t frequency
			);

			virtual ~SPIDisplay() = 0;

			void setup() override;

		protected:
			void setDataCommandPin(bool value) const;
			void setResetPin(bool value) const;

			void writeData(uint8_t data);
			void writeData(const uint8_t *data, size_t length);

			void writeCommand(uint8_t command);

			void writeCommandAndData(uint8_t command, const uint8_t *data, size_t length);
			void writeCommandAndData(uint8_t command, uint8_t data);

			virtual void toggleResetPin();

		private:
			uint8_t _mosiPin;
			uint8_t _misoPin;
			uint8_t _sckPin;
			uint8_t _ssPin;
			uint8_t _dcPin;
			int8_t _rstPin;
			uint32_t _frequency;

	};
}