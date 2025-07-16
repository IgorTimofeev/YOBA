#pragma once

#include "cstdlib"

namespace YOBA {
	enum class ColorModel : uint8_t {
		RGB565,
		RGB666,
		RGB888,
		monochrome,
		HSB,
		palette8Bit
	};

	class RGB888Color;

	class Color {
		public:
			explicit Color(ColorModel model);
			virtual ~Color() = default;

			ColorModel getModel() const;

			static uint8_t getBytesPerModel(ColorModel colorModel);
			static size_t getBytesPerModel(size_t pixelCount, ColorModel colorModel);

			static const Color* select(
				const bool condition,
				const Color* defaultColor,
				const Color* conditionColor,
				const Color* fallbackColor = nullptr
			) {
				if (condition) {
					if (conditionColor)
						return conditionColor;

					if (defaultColor)
						return defaultColor;
				}
				else if (defaultColor) {
					return defaultColor;
				}

				return fallbackColor;
			}

		private:
			ColorModel _model;
	};

	// -------------------------------- HSB --------------------------------

	class HSBColor : Color {
		public:
			HSBColor(float h, float s, float b);

			RGB888Color toRgb888() const;

			float getH() const;
			void setH(float h);

			float getS() const;
			void setS(float s);

			float getB() const;
			void setB(float b);

		private:
			float h = 0;
			float s = 0;
			float b = 0;
	};

	// -------------------------------- ValueColor --------------------------------

	template<typename TValue>
	class ValueColor : public Color {
		public:
			ValueColor(ColorModel model, TValue value);

			TValue getValue() const;
			void setValue(TValue value);

			virtual RGB888Color toRgb888() const = 0;

		protected:
			TValue value;
	};

	template<typename TValue>
	ValueColor<TValue>::ValueColor(const ColorModel model, TValue value) : Color(model), value(value) {

	}

	template<typename TValue>
	TValue ValueColor<TValue>::getValue() const {
		return value;
	}

	template<typename TValue>
	void ValueColor<TValue>::setValue(TValue value) {
		this->value = value;
	}

	// -------------------------------- Monochrome --------------------------------

	class MonochromeColor : public ValueColor<bool> {
		public:
			explicit MonochromeColor(bool value);

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB565 --------------------------------

	class RGB565Color : public ValueColor<uint16_t> {
		public:
			explicit RGB565Color(uint16_t value);

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB666 --------------------------------

	class RGB666Color : public ValueColor<uint32_t> {
		public:
			explicit RGB666Color(uint32_t value);

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB888 --------------------------------

	class RGB888Color : public Color {
		public:
			RGB888Color();
			RGB888Color(uint8_t r, uint8_t g, uint8_t b);
			explicit RGB888Color(uint32_t rgb888);

			uint8_t getR() const;
			void setR(uint8_t r);

			uint8_t getG() const;
			void setG(uint8_t g);

			uint8_t getB() const;
			void setB(uint8_t b);

			uint32_t toUint32() const;
			RGB565Color toRGB565() const;
			RGB666Color toRGB666() const;
			MonochromeColor toMonochrome() const;

			void interpolateTo(const RGB888Color& second, float position);

		private:
			uint8_t r = 0;
			uint8_t g = 0;
			uint8_t b = 0;
	};

	// -------------------------------- PaletteColor --------------------------------

	template<typename TIndex>
	class PaletteColor : public Color {
		public:
			explicit PaletteColor(TIndex index);
			PaletteColor();
			TIndex getIndex() const;
			void setIndex(TIndex value);

		private:
			TIndex _index;
	};

	template<typename TIndex>
	PaletteColor<TIndex>::PaletteColor(TIndex index) : Color(ColorModel::palette8Bit), _index(index) {

	}

	template<typename TIndex>
	PaletteColor<TIndex>::PaletteColor() : PaletteColor(0) {

	}

	template<typename TIndex>
	TIndex PaletteColor<TIndex>::getIndex() const {
		return _index;
	}

	template<typename TIndex>
	void PaletteColor<TIndex>::setIndex(TIndex value) {
		_index = value;
	}

	// -------------------------------- Bit8PaletteColor --------------------------------

	class Bit8PaletteColor : public PaletteColor<uint16_t>{
		public:
			explicit Bit8PaletteColor(uint16_t index);

			Bit8PaletteColor();
	};
}