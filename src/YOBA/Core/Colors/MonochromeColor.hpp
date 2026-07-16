#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	#pragma pack(push, 1)

	class MonochromeColor : public ValueColor<bool> {
		public:
			friend RGB888Color;

			constexpr MonochromeColor(const bool value) : ValueColor(ColorModel::monochrome, value) {

			}

			constexpr RGB888Color toRGB888() const;
	};

	#pragma pack(pop)
}