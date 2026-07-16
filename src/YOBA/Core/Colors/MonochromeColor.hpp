#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	class MonochromeColor : public ValueColor<bool> {
		public:
			constexpr MonochromeColor(const bool value) : ValueColor(ColorModel::monochrome, value) {

			}

			RGB888Color toRGB888() const;
	};
}