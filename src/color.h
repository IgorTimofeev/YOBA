#pragma once

#include "cstdlib"

namespace yoba {
	enum class ColorModel : uint8_t {
		Palette,
		Hsb,
		Monochrome,
		Rgb666,
		Rgb565,
		Rgb888
	};

	class Rgb888Color;

	class Color {
		public:
			explicit Color(ColorModel model);

			ColorModel getModel() const;

			static uint8_t getBytesPerType(ColorModel colorModel);
			static size_t getBytesPerType(size_t pixelsCount, ColorModel colorModel);

		private:
			ColorModel _type;
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
			ValueColor(ColorModel model, TValue value);

			TValue getValue() const;
			void setValue(TValue value);

			virtual Rgb888Color toRgb888() const = 0;

		protected:
			TValue _value;
	};

	template<typename TValue>
	ValueColor<TValue>::ValueColor(ColorModel model, TValue value) : Color(model), _value(value) {

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

	class MonochromeColor : public ValueColor<bool> {
		public:
			explicit MonochromeColor(bool value);

			Rgb888Color toRgb888() const override;
	};

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
			Rgb666Color toRgb666() const;
			MonochromeColor toMonochrome() const;

			void interpolateTo(const Rgb888Color& second, float position);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	// -------------------------------- PaletteColor --------------------------------

	class PaletteColor : public Color {
		public:
			explicit PaletteColor(uint16_t index);

			uint16_t getIndex() const;
			void setIndex(uint16_t index);

		private:
			uint16_t _index;
	};
}