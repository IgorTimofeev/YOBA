#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class RGB888Color;

	#pragma pack(push, 1)

	class HSBColor : public Color {
		public:
			friend RGB888Color;

			constexpr HSBColor(const float h, const float s, const float b) : Color(ColorModel::HSB), _h(h), _s(s), _b(b) {

			}

			constexpr HSBColor() : HSBColor(0, 1.f, 1.f) {

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

			constexpr static void toRGB888(
				const float hsbH,
				const float hsbS,
				const float hsbB,

				uint8_t& rgbR,
				uint8_t& rgbG,
				uint8_t& rgbB
			) {
				const auto hueSector = hsbH * 6.0f;
				const auto hueSectorIntegerPart = static_cast<uint8_t>(hueSector);
				const auto hueSectorFractionalPart = hueSector - static_cast<float>(hueSectorIntegerPart);

				const auto p = static_cast<uint8_t>(255.0f * hsbB * (1 - hsbS));
				const auto q = static_cast<uint8_t>(255.0f * hsbB * (1 - hueSectorFractionalPart * hsbS));
				const auto t = static_cast<uint8_t>(255.0f * hsbB * (1 - (1 - hueSectorFractionalPart) * hsbS));
				const auto v = static_cast<uint8_t>(255.0f * hsbB);

				switch (hueSectorIntegerPart) {
					case 1:  rgbR = q, rgbG = v, rgbB = p; return;
					case 2:  rgbR = p, rgbG = v, rgbB = t; return;
					case 3:  rgbR = p, rgbG = q, rgbB = v; return;
					case 4:  rgbR = t, rgbG = p, rgbB = v; return;
					case 5:  rgbR = v, rgbG = p, rgbB = q; return;
					default: rgbR = v, rgbG = t, rgbB = p; return;
				}
			}

			constexpr RGB888Color toRGB888() const;
			constexpr ARGBColor toARGB() const;
			constexpr RGB565Color toRGB565BE() const;

		private:
			float _h = 0;
			float _s = 0;
			float _b = 0;
	};

	#pragma pack(pop)
}