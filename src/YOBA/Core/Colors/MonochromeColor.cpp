#pragma once

#include <YOBA/Core/Colors/MonochromeColor.hpp>
#include <YOBA/Core/Colors/RGB888Color.hpp>

namespace YOBA {
	RGB888Color MonochromeColor::toRGB888() const {
		return
			_value
				? RGB888Color(0xFF, 0xFF, 0xFF)
				: RGB888Color(0x00, 0x00, 0x00);
	}
}
