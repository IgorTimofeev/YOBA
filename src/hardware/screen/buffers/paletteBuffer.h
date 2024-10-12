#pragma once

#include <cstdlib>
#include <cstring>
#include "screenBuffer.h"

namespace yoba {
	template<typename TIndex, typename TColor, size_t PaletteLength>
	class PaletteBuffer : public ScreenBuffer {
		public:
			explicit PaletteBuffer(ScreenDriver* driver);

			virtual TIndex getPaletteIndexOf(const Color* color);

			void clearNative(const Color* color) override;

			TColor getPaletteColor(TIndex index);
			void setPaletteColor(TIndex index, TColor value);
			void setPaletteColor(TIndex index, const Rgb888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);

		protected:
			TColor _palette[PaletteLength];
	};

	template<typename TIndex, typename TColor, size_t PaletteLength>
	PaletteBuffer<TIndex, TColor, PaletteLength>::PaletteBuffer(ScreenDriver* driver) : ScreenBuffer(driver) {

	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	TIndex PaletteBuffer<TIndex, TColor, PaletteLength>::getPaletteIndexOf(const Color *color) {
		switch (color->getModel()) {
			case ColorModel::Palette:
				return ((PaletteColor<TIndex>*) color)->getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<TIndex, TColor, PaletteLength>::clearNative(const Color* color) {
		memset((TColor*) this->_buffer, (int) getPaletteIndexOf(color), this->_bufferLength);
	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	TColor PaletteBuffer<TIndex, TColor, PaletteLength>::getPaletteColor(TIndex index) {
		return _palette[index];
	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<TIndex, TColor, PaletteLength>::setPaletteColor(TIndex index, TColor value) {
		_palette[index] = value;
	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<TIndex, TColor, PaletteLength>::setPaletteColor(TIndex index, const Rgb888Color &color) {
		setPaletteColor(index, color.toRgb565().getValue());
	}

	template<typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<TIndex, TColor, PaletteLength>::setPaletteColors(std::initializer_list<uint32_t> colors) {
		TIndex index = 0;

		for (auto color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}
}