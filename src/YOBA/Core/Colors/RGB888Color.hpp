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

			constexpr RGB888Color() : RGB888Color(0, 0, 0) {

			}

			constexpr RGB888Color(const uint32_t rgb888) : RGB888Color(
				rgb888 >> 16 & 0xFF,
				rgb888 >> 8 & 0xFF,
				rgb888 & 0xFF
			) {

			}

			uint8_t getR() const;
			void setR(const uint8_t r);

			uint8_t getG() const;
			void setG(const uint8_t g);

			uint8_t getB() const;
			void setB(const uint8_t b);

			uint32_t toUint32() const;

			// RRRRRGGG GGGBBBBB
			RGB565Color toRGB565LE() const;

			// GGGBBBBB RRRRRGGG
			//
			// Most of the displays expects you to send them colors in retarded big-endian format
			// So it's 99.99% you should use this method
			RGB565Color toRGB565BE() const;

			RGB666Color toRGB666() const;
			MonochromeColor toMonochrome() const;
			RGB888Color interpolateTo(const RGB888Color& second, const float position) const;

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;

			uint16_t toUint16RGB565LE() const;
	};
}