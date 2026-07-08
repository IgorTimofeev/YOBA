#pragma once

#include <cstdlib>
#include <bit>

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
			
			constexpr virtual ~Color() = default;

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

	// -------------------------------- ValueColor --------------------------------

	template<typename TValue>
	class ValueColor : public Color {
		public:
			constexpr ValueColor(const ColorModel model, TValue value) : Color(model), _value(value) {
			
			};

			constexpr TValue getValue() const {
				return _value;
			}
			
			constexpr void setValue(TValue value) {
				this->_value = value;
			}

			virtual RGB888Color toRGB888() const = 0;

		protected:
			TValue _value;
	};

	// -------------------------------- Monochrome --------------------------------

	class MonochromeColor : public ValueColor<bool> {
		public:
			constexpr MonochromeColor(const bool value) : ValueColor(ColorModel::monochrome, value) {
			
			}

			RGB888Color toRGB888() const override;
	};

	// -------------------------------- RGB565 --------------------------------

	class RGB565Color : public ValueColor<uint16_t> {
		public:
			constexpr RGB565Color(const uint16_t value) : ValueColor(ColorModel::RGB565, value) {
			
			}

			constexpr RGB565Color(const RGB565Color& value) : ValueColor(ColorModel::RGB565, value._value) {

			}

			RGB888Color toRGB888() const override;
	};

	// -------------------------------- RGB666 --------------------------------

	class RGB666Color : public ValueColor<uint32_t> {
		public:
			constexpr RGB666Color(const uint32_t value) : ValueColor(ColorModel::RGB666, value) {
			
			}

			RGB888Color toRGB888() const override;
	};

	// -------------------------------- RGB888 --------------------------------

	class RGB888Color : public Color {
		public:
			constexpr RGB888Color(const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::RGB888),
				_r(r),
				_g(g),
				_b(b)
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

			constexpr uint8_t getR() const {
				return _r;
			}

			constexpr void setR(const uint8_t r) {
				_r = r;
			}

			constexpr uint8_t getG() const {
				return _g;
			}

			constexpr void setG(const uint8_t g) {
				_g = g;
			}

			constexpr uint8_t getB() const {
				return _b;
			}

			constexpr void setB(const uint8_t b) {
				_b = b;
			}

			constexpr uint32_t toUint32() const {
				return _r << 16 | _g << 8 | _b;
			}

			// RRRRRGGG GGGBBBBB
			constexpr RGB565Color toRGB565LE() const {
				return { toUint16RGB565LE() };
			}

			// GGGBBBBB RRRRRGGG
			//
			// Most of the displays expects you to send them colors in retarded big-endian format
			// So it's 99.99% you should use this method
			constexpr RGB565Color toRGB565BE() const {
				return { std::byteswap(toUint16RGB565LE()) };
			}

			RGB666Color toRGB666() const;
			MonochromeColor toMonochrome() const;

			void interpolateTo(const RGB888Color& second, float position);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;

			constexpr uint16_t toUint16RGB565LE() const {
				return ((_r & 0b11111000) << 8) | ((_g & 0b11111100) << 3) | (_b >> 3);
			}
	};

	// -------------------------------- PaletteColor --------------------------------

	template<typename TIndex>
	class PaletteColor : public Color {
		public:
			constexpr PaletteColor(TIndex index) : Color(ColorModel::palette8Bit), _index(index) {
			
			}
			
			constexpr PaletteColor() : PaletteColor(0) {
			
			}
			
			constexpr TIndex getIndex() const {
				return _index;
			}
			
			constexpr void setIndex(TIndex value) {
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

	// -------------------------------- HSBColor --------------------------------

	class HSBColor : Color {
		public:
			constexpr HSBColor(const float h, const float s, const float b) : Color(ColorModel::HSB), _h(h), _s(s), _b(b) {

			}

			constexpr float getH() const {
				return _h;
			}

			constexpr void setH(const float h) {
				_h = h;
			}

			constexpr float getS() const {
				return _s;
			}

			constexpr void setS(const float s) {
				_s = s;
			}

			constexpr float getB() const {
				return _b;
			}

			constexpr void setB(const float b) {
				_b = b;
			}

			constexpr RGB888Color toRGB888() const {
				const auto hueSector = _h * 6.0f;
				const auto hueSectorIntegerPart = static_cast<uint8_t>(hueSector);
				const auto hueSectorFractionalPart = hueSector - static_cast<float>(hueSectorIntegerPart);

				const auto p = static_cast<uint8_t>(255.0f * _b * (1 - _s));
				const auto q = static_cast<uint8_t>(255.0f * _b * (1 - hueSectorFractionalPart * _s));
				const auto t = static_cast<uint8_t>(255.0f * _b * (1 - (1 - hueSectorFractionalPart) * _s));
				const auto v = static_cast<uint8_t>(255.0f * _b);

				switch (hueSectorIntegerPart) {
					case 1: return { q, v, p };
					case 2: return { p, v, t};
					case 3: return { p, q, v };
					case 4: return { t, p, v };
					case 5: return { v, p, q };
					default: return { v, t, p };
				}
			}

		private:
			float _h = 0;
			float _s = 0;
			float _b = 0;
	};



}