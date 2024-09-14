#pragma once

#include <cstdlib>
#include <cstring>
#include "renderBuffer.h"

namespace yoba {
	template<typename TIndex>
	class PaletteBuffer : public RenderBuffer<TIndex> {
		public:
			PaletteBuffer(Driver *driver, const Size &resolution, uint16_t *palette);

			void setPaletteColor(TIndex index, uint16_t paletteIndex);
			void clearNative(TIndex paletteIndex) override;

		protected:
			uint16_t* _palette;
	};

	template<typename TIndex>
	PaletteBuffer<TIndex>::PaletteBuffer(Driver *driver, const Size &resolution, uint16_t *palette) : RenderBuffer<TIndex>(driver, resolution), _palette(palette) {

	}

	template<typename TIndex>
	void PaletteBuffer<TIndex>::setPaletteColor(TIndex index, uint16_t paletteIndex) {
		_palette[index] = paletteIndex;
	}

	template<typename TIndex>
	void PaletteBuffer<TIndex>::clearNative(TIndex paletteIndex) {
		memset((uint16_t*) this->_buffer, (int) paletteIndex, this->_bufferLength);
	}
}