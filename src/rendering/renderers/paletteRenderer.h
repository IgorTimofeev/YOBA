#pragma once

#include <cstdlib>
#include <cstring>
#include "renderer.h"

namespace yoba {
	class PaletteRenderer : public Renderer {
		public:
			PaletteRenderer(uint16_t paletteLength);

			uint32_t getPaletteColor(uint16_t index);
			void setPaletteColor(uint16_t index, uint32_t value);
			void setPaletteColor(uint16_t index, const Rgb888Color& color);

			void setPaletteColors(std::initializer_list<uint32_t> colors);
			void setPaletteColors(std::initializer_list<Rgb888Color> colors);

			virtual uint16_t getPaletteIndex(const Color* color);

		protected:
			void onTargetChanged() override;

			uint16_t getPaletteLength() const;
			uint8_t* getPalette() const;

		private:
			uint16_t _paletteLength;
			uint8_t* _palette = nullptr;
	};
}