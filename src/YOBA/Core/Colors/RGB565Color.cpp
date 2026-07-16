#pragma once

#include <YOBA/Core/Colors/RGB565Color.hpp>
#include <YOBA/Core/Colors/RGB888Color.hpp>

namespace YOBA {
	RGB888Color RGB565Color::toRGB888LE() const {
		return {
			static_cast<uint8_t>(((_value >> 11 & 0x1F) * 255 + 15) / 31),
			static_cast<uint8_t>(((_value >> 5 & 0x3F) * 255 + 31) / 63),
			static_cast<uint8_t>(((_value & 0x1F) * 255 + 15) / 31)
		};
	}
}
