#pragma once

#include <cstdlib>
#include <cstring>
#include "screenBuffer.h"

namespace yoba {
	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	class PaletteBuffer : public TypedScreenBuffer<ColorType> {
		public:
			explicit PaletteBuffer(TypedScreenDriver<ColorType>* driver);

			virtual TIndex getPaletteIndexOf(const Color* color);

			void clearNative(const Color* color) override;

			TColor getPaletteColor(TIndex index);
			void setPaletteColor(TIndex index, TColor value);
			void setPaletteColor(TIndex index, const Rgb888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);

		protected:
			TColor _palette[PaletteLength];
	};

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::PaletteBuffer(TypedScreenDriver<ColorType>* driver) : TypedScreenBuffer<ColorType>(driver) {

	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	TIndex PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::getPaletteIndexOf(const Color *color) {
		switch (color->getType()) {
			case ColorType::Palette:
				return ((PaletteColor<TIndex>*) color)->getIndex();

			default:
				return 0;
		}
	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::clearNative(const Color* color) {
		memset((TColor*) this->_buffer, (int) getPaletteIndexOf(color), this->_bufferLength);
	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	TColor PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::getPaletteColor(TIndex index) {
		return _palette[index];
	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::setPaletteColor(TIndex index, TColor value) {
		_palette[index] = value;
	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::setPaletteColor(TIndex index, const Rgb888Color &color) {
		setPaletteColor(index, color.toRgb565().getValue());
	}

	template<ColorType ColorType, typename TIndex, typename TColor, size_t PaletteLength>
	void PaletteBuffer<ColorType, TIndex, TColor, PaletteLength>::setPaletteColors(std::initializer_list<uint32_t> colors) {
		TIndex index = 0;

		for (auto color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}
}