#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	class RGB666Color : public ValueColor<uint32_t> {
		public:
			constexpr RGB666Color(const uint32_t value) : ValueColor(ColorModel::RGB666, value) {

			}

			constexpr RGB888Color toRGB888() const;
	};
}