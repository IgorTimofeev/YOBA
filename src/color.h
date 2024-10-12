#pragma once

#include "cstdlib"

namespace yoba {
	enum class ColorDepth : uint8_t {
		Bit1,
		Bit8,
		Bit16,
		Bit32
	};

	enum class ColorType : uint8_t {
		Palette,
		Hsb,
		Rgb565,
		Rgb666,
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

	// -------------------------------- ValueColor --------------------------------

	template<typename TValue>
	class ValueColor : public Color {
		public:
			ValueColor(ColorType type, TValue value);

			TValue getValue() const;
			void setValue(TValue value);

			virtual Rgb888Color toRgb888() const = 0;

		protected:
			TValue _value;
	};

	template<typename TValue>
	ValueColor<TValue>::ValueColor(ColorType type, TValue value) : Color(type), _value(value) {

	}

	template<typename TValue>
	TValue ValueColor<TValue>::getValue() const {
		return _value;
	}

	template<typename TValue>
	void ValueColor<TValue>::setValue(TValue value) {
		_value = value;
	}

	// -------------------------------- Rgb565Color --------------------------------

	class Rgb565Color : public ValueColor<uint16_t> {
		public:
			explicit Rgb565Color(uint16_t value);

			Rgb888Color toRgb888() const override;
	};

	// -------------------------------- Rgb666Color --------------------------------

	class Rgb666Color : public ValueColor<uint32_t> {
		public:
			explicit Rgb666Color(uint32_t value);

			Rgb888Color toRgb888() const override;
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