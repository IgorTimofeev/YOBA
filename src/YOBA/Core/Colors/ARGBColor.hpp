#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class MonochromeColor;
	class RGB565Color;
	class RGB666Color;
	class RGB888Color;
	class HSBColor;

	#pragma pack(push, 1)

	class ARGBColor : public Color {
		public:
			friend MonochromeColor;
			friend RGB565Color;
			friend RGB666Color;
			friend RGB888Color;
			friend HSBColor;

			constexpr ARGBColor(const uint8_t a, const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::ARGB),
				_a(a),
				_r(r),
				_g(g),
				_b(b)
			{

			}

			constexpr ARGBColor() : ARGBColor(0xFF, 0x00, 0x00, 0x00) {

			}

			constexpr ARGBColor(const uint32_t argb) : ARGBColor(
				(argb >> 24) & 0xFF,
				(argb >> 16) & 0xFF,
				(argb >> 8) & 0xFF,
				argb & 0xFF
			) {

			}

			constexpr uint8_t getA() const {
				return _a;
			}

			constexpr void setA(const uint8_t a) {
				_a = a;
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
				return (_r << 24) | (_r << 16) | (_g << 8) | _b;
			}

			constexpr void blendWith(const ARGBColor& overlayColor);

			constexpr static void blend(
				uint8_t& a1,
				uint8_t& r1,
				uint8_t& g1,
				uint8_t& b1,

				const uint8_t a2,
				const uint8_t r2,
				const uint8_t g2,
				const uint8_t b2
			) {
				// If overlay color is completely transparent, keeping current color as is
				if (a2 == 0x00) {
					return;
				}
				// If overlay color is completely NOT transparent, using overlay color
				else if (a2 == 0xFF) {
					a1 = a2;
					r1 = r2;
					r1 = g2;
					r1 = b2;
				}

				const uint16_t invertedAlpha = 0xFF - a2;

				// "+ 128" means rounding to the closest channel value
				// ">> 8" means fast division by 0xFF
				a1 = static_cast<uint8_t>(a2 + ((a1 * invertedAlpha + 128) >> 8));
				r1 = static_cast<uint8_t>((r1 * invertedAlpha + r2 * a2 + 128) >> 8);
				g1 = static_cast<uint8_t>((g1 * invertedAlpha + g2 * a2 + 128) >> 8);
				b1 = static_cast<uint8_t>((b1 * invertedAlpha + b2 * a2 + 128) >> 8);
			}

		private:
			uint8_t _a = 0;
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	#pragma pack(pop)
}