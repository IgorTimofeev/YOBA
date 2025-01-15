#pragma once

#include <cstdlib>
#include <cstring>
#include "bufferedRenderer.h"

namespace yoba {
	class PaletteBufferedRenderer : public BufferedRenderer {
		public:
			PaletteBufferedRenderer(RenderTarget* renderTarget, uint16_t paletteLength);

			uint32_t getPaletteColor(uint16_t index);
			void setPaletteColor(uint16_t index, uint32_t value);
			void setPaletteColor(uint16_t index, const Rgb888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);

			virtual uint16_t getPaletteIndex(const Color* color);

		protected:
			uint16_t _paletteLength;
			uint8_t* _palette = nullptr;

			void allocate() override;
	};
}