#pragma once

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

	constexpr RGB888Color RGB565Color::toRGB888LE() const {
		return {
			static_cast<uint8_t>(((_value >> 11 & 0x1F) * 255 + 15) / 31),
			static_cast<uint8_t>(((_value >> 5 & 0x3F) * 255 + 31) / 63),
			static_cast<uint8_t>(((_value & 0x1F) * 255 + 15) / 31)
		};
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

	constexpr RGB565Color RGB888Color::toRGB565LE() const {
		return { toUint16RGB565LE() };
	}

	constexpr RGB565Color RGB888Color::toRGB565BE() const {
		return { std::byteswap(toUint16RGB565LE()) };
	}

	constexpr RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color((_r << 16) | (_g << 8) | _b);
	}

	constexpr MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor(_r > 0 || _g > 0 || _b > 0);
	}

	constexpr void RGB888Color::interpolateTo(const RGB888Color& second, const float position) {
		_r = static_cast<uint8_t>(Math::interpolate(_r, second._r, position));
		_g = static_cast<uint8_t>(Math::interpolate(_g, second._g, position));
		_b = static_cast<uint8_t>(Math::interpolate(_b, second._b, position));
	}

	constexpr uint16_t RGB888Color::toUint16RGB565LE() const {
		return ((_r & 0b11111000) << 8) | ((_g & 0b11111100) << 3) | (_b >> 3);
	}

	constexpr void RGB888Color::blendWith(const ARGBColor& overlayColor) {
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

	// -------------------------------- ARGB --------------------------------

	constexpr void ARGBColor::blendWith(const ARGBColor& overlayColor) {
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

	constexpr RGB565Color HSBColor::toRGB565BE() const {
		return toRGB888().toRGB565BE();
	}
}