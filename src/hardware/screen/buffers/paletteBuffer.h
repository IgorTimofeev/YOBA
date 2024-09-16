#pragma once

#include <cstdlib>
#include <cstring>
#include "screenBuffer.h"

namespace yoba {
	class PaletteBuffer : public ScreenBuffer {
		public:
			PaletteBuffer(ScreenDriver *driver, uint16_t *palette);

			virtual uint8_t getPaletteIndex(const Color* color);

			void clearUnchecked(const Color* color) override;

			uint16_t getPaletteColor(uint8_t index);
			void setPaletteColor(uint8_t index, uint16_t value);

		protected:
			uint16_t* _palette;
	};
}