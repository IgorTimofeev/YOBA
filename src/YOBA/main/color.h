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
			constexpr Color(const ColorModel model) : _model(model) {
			
			}
			
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
			constexpr HSBColor(const float h, const float s, const float b) : Color(ColorModel::HSB), h(h), s(s), b(b) {
			
			}

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
			constexpr ValueColor(const ColorModel model, TValue value) : Color(model), _value(value) {
			
			};

			TValue getValue() const {
				return _value;
			}
			
			void setValue(TValue value) {
				this->_value = value;
			}

			virtual RGB888Color toRgb888() const = 0;

		protected:
			TValue _value;
	};

	// -------------------------------- Monochrome --------------------------------

	class MonochromeColor : public ValueColor<bool> {
		public:
			constexpr MonochromeColor(const bool value) : ValueColor(ColorModel::monochrome, value) {
			
			}

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB565 --------------------------------

	class RGB565Color : public ValueColor<uint16_t> {
		public:
			constexpr RGB565Color(const uint16_t value) : ValueColor(ColorModel::RGB565, value) {
			
			}

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB666 --------------------------------

	class RGB666Color : public ValueColor<uint32_t> {
		public:
			constexpr RGB666Color(const uint32_t value) : ValueColor(ColorModel::RGB666, value) {
			
			}

			RGB888Color toRgb888() const override;
	};

	// -------------------------------- RGB888 --------------------------------

	class RGB888Color : public Color {
		public:
			constexpr RGB888Color(const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::RGB888),
				r(r),
				g(g),
				b(b)
			{
			
			}
			
			constexpr RGB888Color() : RGB888Color(0, 0, 0) {
			
			}
			
			constexpr RGB888Color(const uint32_t rgb888) : RGB888Color(
				rgb888 >> 16 & 0xFF,
				rgb888 >> 8 & 0xFF,
				rgb888 & 0xFF
			) {
			
			}

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
			constexpr PaletteColor(TIndex index) : Color(ColorModel::palette8Bit), _index(index) {
			
			}
			
			constexpr PaletteColor() : PaletteColor(0) {
			
			}
			
			TIndex getIndex() const {
				return _index;
			}
			
			void setIndex(TIndex value) {
				_index = value;
			}
		
		private:
			TIndex _index;
	};


	// -------------------------------- Bit8PaletteColor --------------------------------

	class Bit8PaletteColor : public PaletteColor<uint16_t> {
		public:
			constexpr Bit8PaletteColor(const uint16_t index) : PaletteColor(index) {
			
			}
			
			constexpr Bit8PaletteColor() : Bit8PaletteColor(0) {
			
			}
	};
}