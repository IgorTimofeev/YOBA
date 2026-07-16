#pragma once

#include <YOBA/Core/Colors/ValueColor.hpp>

namespace YOBA {
	class RGB888Color;

	#pragma pack(push, 1)

	class RGB666Color : public ValueColor<uint32_t> {
		friend RGB888Color;

		public:
			constexpr RGB666Color(const uint32_t value) : ValueColor(ColorModel::RGB666, value) {

			}

			constexpr RGB888Color toRGB888() const;
	};

	#pragma pack(pop)
}