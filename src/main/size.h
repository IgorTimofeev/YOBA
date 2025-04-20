#pragma once

#include <cstdint>
#include "vector2.h"

namespace yoba {
	struct Size {
		public:
			Size(uint16_t width, uint16_t height);
			Size(uint16_t uniformValue);
			Size();

			constexpr static const uint16_t computed = 0xFFFF;
			constexpr static const uint16_t unlimited = 0xFFFE;

			uint16_t getWidth() const;
			void setWidth(uint16_t value);

			uint16_t getHeight() const;
			void setHeight(uint16_t value);

			uint16_t getXCenter() const;
			uint16_t getYCenter() const;
			Point getCenter() const;
			uint32_t getSquare() const;

			Size interpolate(const Size& to, const double &position) const;

			bool operator==(const Size& rhs) const;
			bool operator!=(const Size& rhs) const;

		private:
			uint16_t _width;
			uint16_t _height;
	};
}