#pragma once

#include <cstdlib>
#include <cstring>
#include "screenBuffer.h"

namespace yoba {
	template<typename TIndex, typename TColor, size_t TPaletteSize>
	class PaletteBuffer : public ScreenBuffer {
		public:
			explicit PaletteBuffer(ScreenDriver *driver);

			virtual TIndex getPaletteIndexOf(const Color* color);

			void clearNative(const Color* color) override;

			TColor getPaletteColor(TIndex index);
			void setPaletteColor(TIndex index, TColor value);

		protected:
			TColor _palette[TPaletteSize];
	};

	template<typename TIndex, typename TColor, size_t TPaletteSize>
	PaletteBuffer<TIndex, TColor, TPaletteSize>::PaletteBuffer(ScreenDriver *driver) : ScreenBuffer(driver) {

	}

	template<typename TIndex, typename TColor, size_t TPaletteSize>
	TIndex PaletteBuffer<TIndex, TColor, TPaletteSize>::getPaletteIndexOf(const Color *color) {
		switch (color->getType()) {
			case ColorType::Palette:
				return ((PaletteColor*) color)->getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TColor, size_t TPaletteSize>
	void PaletteBuffer<TIndex, TColor, TPaletteSize>::clearNative(const Color* color) {
		memset((TColor*) this->_buffer, (int) getPaletteIndexOf(color), this->_bufferLength);
	}

	template<typename TIndex, typename TColor, size_t TPaletteSize>
	TColor PaletteBuffer<TIndex, TColor, TPaletteSize>::getPaletteColor(TIndex index) {
		return _palette[index];
	}

	template<typename TIndex, typename TColor, size_t TPaletteSize>
	void PaletteBuffer<TIndex, TColor, TPaletteSize>::setPaletteColor(TIndex index, TColor value) {
		_palette[index] = value;
	}
}