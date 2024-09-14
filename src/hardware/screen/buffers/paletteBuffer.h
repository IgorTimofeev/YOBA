#pragma once

#include <cstdlib>
#include <cstring>
#include "renderBuffer.h"

namespace yoba {
	template<typename TIndex>
	class PaletteBuffer : public RenderBuffer<TIndex> {
		public:
			PaletteBuffer(ScreenDriver *driver, const Size &resolution, uint16_t *palette);

			void clearNative(TIndex paletteIndex) override;

			TIndex getPaletteColor(size_t index);
			void setPaletteColor(size_t index, TIndex value);

		protected:
			uint16_t* _palette;
	};

	template<typename TIndex>
	PaletteBuffer<TIndex>::PaletteBuffer(ScreenDriver *driver, const Size &resolution, uint16_t *palette) : RenderBuffer<TIndex>(driver, resolution), _palette(palette) {

	}

	template<typename TIndex>
	void PaletteBuffer<TIndex>::clearNative(TIndex paletteIndex) {
		memset((uint16_t*) this->_buffer, (int) paletteIndex, this->_bufferLength);
	}

	template<typename TIndex>
	TIndex PaletteBuffer<TIndex>::getPaletteColor(size_t index) {
		return _palette[index];
	}

	template<typename TIndex>
	void PaletteBuffer<TIndex>::setPaletteColor(size_t index, TIndex value) {
		_palette[index] = value;
	}
}