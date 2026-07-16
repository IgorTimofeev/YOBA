#pragma once

#include <YOBA/Core/Colors/RGB666Color.hpp>
#include <YOBA/Core/Colors/RGB888Color.hpp>

namespace YOBA {
	RGB888Color RGB666Color::toRGB888() const {
		return {
			static_cast<uint8_t>((_value >> 12 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value >> 6 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value & 0b111111) * 255 / 0b111111)
		};
	}
}
