#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class MonochromeColor;
	class RGB565Color;
	class RGB666Color;
	class RGB888Color;

	class RGB888Color : public Color {
		public:
			constexpr RGB888Color(const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::RGB888),
				_r(r),
				_g(g),
				_b(b)
			{

			}

			constexpr RGB888Color() : RGB888Color(0x00, 0x00, 0x00) {

			}

			constexpr RGB888Color(const uint32_t rgb888) : RGB888Color(
				(rgb888 >> 16) & 0xFF,
				(rgb888 >> 8) & 0xFF,
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
				return (_r << 16) | (_g << 8) | _b;
			}

			// RRRRRGGG GGGBBBBB
			constexpr RGB565Color toRGB565LE() const;

			// GGGBBBBB RRRRRGGG
			//
			// Most of the displays expects you to send them colors in retarded big-endian format
			// So it's 99.99% you should use this method
			constexpr RGB565Color toRGB565BE() const;

			constexpr RGB666Color toRGB666() const;
			constexpr MonochromeColor toMonochrome() const;
			constexpr RGB888Color interpolateTo(const RGB888Color& second, const float position) const;

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;

			constexpr uint16_t toUint16RGB565LE() const;
	};
}