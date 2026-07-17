#pragma once

#include <functional>

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class RGB888Color;

	#pragma pack(push, 1)

	class HSBColor : public Color {
		public:
			friend RGB888Color;

			constexpr HSBColor(const float h, const float s, const float b) : Color(ColorModel::HSB), _h(h), _s(s), _b(b) {

			}

			constexpr HSBColor() : HSBColor(0.0f, 1.f, 1.f) {

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
				const float hue,
				const float saturation,
				const float brightness,

				uint8_t& red,
				uint8_t& green,
				uint8_t& blue
			) {
				const auto hueSector = hue * 6.0f;
				const auto hueSectorIntegerPart = static_cast<uint8_t>(hueSector);
				const auto hueSectorFractionalPart = hueSector - static_cast<float>(hueSectorIntegerPart);

				const auto p = static_cast<uint8_t>(255.0f * brightness * (1 - saturation));
				const auto q = static_cast<uint8_t>(255.0f * brightness * (1 - hueSectorFractionalPart * saturation));
				const auto t = static_cast<uint8_t>(255.0f * brightness * (1 - (1 - hueSectorFractionalPart) * saturation));
				const auto v = static_cast<uint8_t>(255.0f * brightness);

				switch (hueSectorIntegerPart) {
					case 1:  red = q, green = v, blue = p; return;
					case 2:  red = p, green = v, blue = t; return;
					case 3:  red = p, green = q, blue = v; return;
					case 4:  red = t, green = p, blue = v; return;
					case 5:  red = v, green = p, blue = q; return;
					default: red = v, green = t, blue = p; return;
				}
			}

			constexpr RGB888Color toRGB888() const;
			constexpr ARGBColor toARGB() const;
			constexpr RGB565Color toRGB565() const;
			constexpr void toModel(const ColorModel targetModel, const std::function<void(const Color* color)>& convertedColorHandler) const;

		private:
			float _h = 0;
			float _s = 0;
			float _b = 0;
	};

	#pragma pack(pop)
}