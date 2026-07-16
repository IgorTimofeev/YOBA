#pragma once

#include <YOBA/Core/Colors/RGB888Color.hpp>
#include <YOBA/Core/Colors/RGB565Color.hpp>
#include <YOBA/Core/Colors/RGB666Color.hpp>
#include <YOBA/Core/Colors/MonochromeColor.hpp>
#include <YOBA/Core/Math.hpp>

namespace YOBA {
	uint8_t RGB888Color::getR() const {
		return _r;
	}

	void RGB888Color::setR(const uint8_t r) {
		_r = r;
	}

	uint8_t RGB888Color::getG() const {
		return _g;
	}

	void RGB888Color::setG(const uint8_t g) {
		_g = g;
	}

	uint8_t RGB888Color::getB() const {
		return _b;
	}

	void RGB888Color::setB(const uint8_t b) {
		_b = b;
	}

	uint32_t RGB888Color::toUint32() const {
		return (_r << 16) | (_g << 8) | _b;
	}

	RGB565Color RGB888Color::toRGB565LE() const {
		return { toUint16RGB565LE() };
	}

	RGB565Color RGB888Color::toRGB565BE() const {
		return { std::byteswap(toUint16RGB565LE()) };
	}

	RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color((_r << 16) | (_g << 8) | _b);
	}

	MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor(_r > 0 || _g > 0 || _b > 0);
	}

	RGB888Color RGB888Color::interpolateTo(const RGB888Color& second, const float position) const {
		return {
			static_cast<uint8_t>(Math::interpolate(_r, second._r, position)),
			static_cast<uint8_t>(Math::interpolate(_g, second._g, position)),
			static_cast<uint8_t>(Math::interpolate(_b, second._b, position))
		};
	}

	uint16_t RGB888Color::toUint16RGB565LE() const {
		return ((_r & 0b11111000) << 8) | ((_g & 0b11111100) << 3) | (_b >> 3);
	}
}
