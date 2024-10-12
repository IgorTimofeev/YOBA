#pragma once

#include "cstdlib"

namespace yoba {
	enum class ColorType : uint8_t {
		Palette,
		Hsb,
		Rgb565,
		Rgb888
	};

	class Rgb888Color;

	class Color {
		public:
			explicit Color(ColorType type);

			ColorType getType() const;

		private:
			ColorType _type;
	};

	// -------------------------------- HsbColor --------------------------------

	class HsbColor : Color {
		public:
			HsbColor(float h, float s, float b);

			Rgb888Color toRgb888() const;

			float getH() const;
			void setH(float h);

			float getS() const;
			void setS(float s);

			float getB() const;
			void setB(float b);

		private:
			float _h = 0;
			float _s = 0;
			float _b = 0;
	};

	// -------------------------------- Rgb565Color --------------------------------

	class Rgb565Color : public Color {
		public:
			explicit Rgb565Color(uint16_t value);

			Rgb565Color();

			uint16_t getValue() const;
			void setValue(uint16_t value);

			Rgb888Color toRgb888() const;

		private:
			uint16_t _value;
	};

	// -------------------------------- Rgb888Color --------------------------------

	class Rgb888Color : public Color {
		public:
			Rgb888Color();

			Rgb888Color(uint8_t r, uint8_t g, uint8_t b);
			Rgb888Color(const Rgb888Color &source);
			explicit Rgb888Color(uint32_t rgb888);

			uint8_t getR() const;
			void setR(uint8_t r);

			uint8_t getG() const;
			void setG(uint8_t g);

			uint8_t getB() const;
			void setB(uint8_t b);

			uint32_t toUint32() const;
			Rgb565Color toRgb565() const;

			void interpolateTo(const Rgb888Color& second, float position);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	// -------------------------------- PaletteColor --------------------------------

	template<typename TIndex>
	class PaletteColor : public Color {
		public:
			explicit PaletteColor(TIndex index);

			TIndex getIndex() const;
			void setIndex(TIndex index);

		private:
			TIndex _index;
	};

	template<typename TIndex>
	PaletteColor<TIndex>::PaletteColor(TIndex index) : Color(ColorType::Palette), _index(index) {

	}

	template<typename TIndex>
	TIndex PaletteColor<TIndex>::getIndex() const {
		return _index;
	}

	template<typename TIndex>
	void PaletteColor<TIndex>::setIndex(TIndex index) {
		_index = index;
	}

	// -------------------------------- Bit8PaletteColor --------------------------------

	using Bit8PaletteColor = PaletteColor<uint8_t>;
}