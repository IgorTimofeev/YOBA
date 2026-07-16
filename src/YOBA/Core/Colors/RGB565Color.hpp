#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	#pragma pack(push, 1)

	class RGB565Color : public ValueColor<uint16_t> {
		public:
			friend RGB888Color;

			constexpr RGB565Color(const uint16_t value) : ValueColor(ColorModel::RGB565, value) {

			}

			constexpr RGB565Color() : ValueColor(ColorModel::RGB565, 0) {

			}

			constexpr RGB888Color toRGB888LE() const;
	};

	#pragma pack(pop)
}