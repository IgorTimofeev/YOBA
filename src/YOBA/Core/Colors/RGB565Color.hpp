#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	class RGB565Color : public ValueColor<uint16_t> {
		public:
			constexpr RGB565Color(const uint16_t value) : ValueColor(ColorModel::RGB565, value) {

			}

			constexpr RGB565Color() : ValueColor(ColorModel::RGB565, 0) {

			}

			constexpr RGB888Color toRGB888LE() const;
	};
}