#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class MonochromeColor;
	class RGB565Color;
	class RGB666Color;
	class ARGB8888Color;

	class ARGB8888Color : public Color {
		public:
			constexpr ARGB8888Color(const uint8_t a, const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::RGB888),
				_a(a),
				_r(r),
				_g(g),
				_b(b)
			{

			}

			constexpr ARGB8888Color() : ARGB8888Color(0xFF, 0x00, 0x00, 0x00) {

			}

			constexpr ARGB8888Color(const uint32_t rgb888) : ARGB8888Color(
				(rgb888 >> 24) & 0xFF,
				(rgb888 >> 16) & 0xFF,
				(rgb888 >> 8) & 0xFF,
				rgb888 & 0xFF
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

		private:
			uint8_t _a = 0;
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};
}