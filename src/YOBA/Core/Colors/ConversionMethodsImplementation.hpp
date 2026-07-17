#pragma once

#include <bit>

#include <YOBA/Core/Colors/Color.hpp>
#include <YOBA/Core/Colors/MonochromeColor.hpp>
#include <YOBA/Core/Colors/RGB565Color.hpp>
#include <YOBA/Core/Colors/RGB666Color.hpp>
#include <YOBA/Core/Colors/RGB888Color.hpp>
#include <YOBA/Core/Colors/ARGBColor.hpp>
#include <YOBA/Core/Colors/HSBColor.hpp>
#include <YOBA/Core/Math.hpp>

namespace YOBA {
	// -------------------------------- RGB565 --------------------------------

	constexpr RGB888Color MonochromeColor::toRGB888() const {
		return
			_value
				? RGB888Color(0xFF, 0xFF, 0xFF)
				: RGB888Color(0x00, 0x00, 0x00);
	}

	// -------------------------------- RGB565 --------------------------------

	constexpr RGB888Color RGB565Color::toRGB888() const {
		const uint8_t r5 = (_value >> 11) & 0b0001'1111;
		const uint8_t g6 = (_value >> 5)  & 0b0011'1111;
		const uint8_t b5 = _value         & 0b0001'1111;

		return RGB888Color(
			(r5 << 3) | (r5 >> 2),
			(g6 << 2) | (g6 >> 4),
			(b5 << 3) | (b5 >> 2)
		);
	}

	// -------------------------------- RGB666 --------------------------------

	constexpr RGB888Color RGB666Color::toRGB888() const {
		return {
			static_cast<uint8_t>((_value >> 12 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value >> 6 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value & 0b111111) * 255 / 0b111111)
		};
	}

	// -------------------------------- RGB888 --------------------------------

	constexpr void RGB888Color::interpolate(const RGB888Color& second, const float position) {
		_r = static_cast<uint8_t>(Math::interpolate(_r, second._r, position));
		_g = static_cast<uint8_t>(Math::interpolate(_g, second._g, position));
		_b = static_cast<uint8_t>(Math::interpolate(_b, second._b, position));
	}

	constexpr void RGB888Color::blend(const ARGBColor& overlayColor) {
		blend(
			_r,
			_g,
			_b,

			overlayColor._a,
			overlayColor._r,
			overlayColor._g,
			overlayColor._b
		);
	}

	constexpr MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor((static_cast<uint16_t>(_r) + _g + _b) / 3 >= 127);
	}

	constexpr RGB565Color RGB888Color::toRGB565() const {
		return RGB565Color(((_r & 0b11111000) << 8) | ((_g & 0b11111100) << 3) | (_b >> 3));
	}

	constexpr RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color((_r << 16) | (_g << 8) | _b);
	}

	constexpr ARGBColor RGB888Color::toARGB() const {
		return ARGBColor(0xFF, _r, _g, _b);
	}

	constexpr HSBColor RGB888Color::toHSB() const {
		HSBColor result {};

		toHSB(
			_r,
			_g,
			_b,

			result._h,
			result._s,
			result._b
		);

		return result;
	}

	constexpr void RGB888Color::toModel(const ColorModel targetModel, const std::function<void(const Color* color)>& convertedColorHandler) const {
		switch (targetModel) {
			case ColorModel::RGB565: {
				const auto color = toRGB565();
				convertedColorHandler(&color);

				break;
			}
			case ColorModel::ARGB: {
				const auto color = toARGB();
				convertedColorHandler(&color);

				break;
			}
			case ColorModel::HSB: {
				const auto color = toHSB();
				convertedColorHandler(&color);

				break;
			}
			default:
				break;
		}
	}

	// -------------------------------- ARGB --------------------------------

	constexpr void ARGBColor::blend(const ARGBColor& overlayColor) {
		blend(
			_a,
			_r,
			_g,
			_b,

			overlayColor._a,
			overlayColor._r,
			overlayColor._g,
			overlayColor._b
		);
	}

	// -------------------------------- HSB --------------------------------

	constexpr RGB888Color HSBColor::toRGB888() const {
		RGB888Color result {};

		toRGB888(
			_h,
			_s,
			_b,

			result._r,
			result._g,
			result._b
		);

		return result;
	}

	constexpr ARGBColor HSBColor::toARGB() const {
		ARGBColor result {};

		toRGB888(
			_h,
			_s,
			_b,

			result._r,
			result._g,
			result._b
		);

		return result;
	}

	constexpr RGB565Color HSBColor::toRGB565() const {
		return toRGB888().toRGB565();
	}

	constexpr void HSBColor::toModel(const ColorModel targetModel, const std::function<void(const Color* color)>& convertedColorHandler) const {
		switch (targetModel) {
			case ColorModel::RGB565: {
				const auto color = toRGB565();
				convertedColorHandler(&color);

				break;
			}
			case ColorModel::RGB888: {
				const auto color = toRGB888();
				convertedColorHandler(&color);

				break;
			}
			case ColorModel::ARGB: {
				const auto color = toARGB();
				convertedColorHandler(&color);

				break;
			}
			default:
				break;
		}
	}
}