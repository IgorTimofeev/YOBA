#pragma once

#include <YOBA/Core/Colors/HSBColor.hpp>
#include <YOBA/Core/Colors/RGB888Color.hpp>

namespace YOBA {
	RGB888Color HSBColor::toRGB888() const {
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
}
