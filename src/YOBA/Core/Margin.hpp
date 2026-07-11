#pragma once

#include <cstdint>

namespace YOBA {
	class Margin {
		public:
			constexpr Margin(const uint16_t left, const uint16_t top, const uint16_t right, const uint16_t bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {

			}

			constexpr Margin(const uint16_t horizontal, const uint16_t vertical) : _left(horizontal), _top(vertical), _right(horizontal), _bottom(vertical) {

			}

			constexpr Margin(const uint16_t uniformValue) : _left(uniformValue), _top(uniformValue), _right(uniformValue), _bottom(uniformValue) {

			}

			constexpr Margin() : _left(0), _top(0), _right(0), _bottom(0) {

			}

			static Margin zero;

			uint16_t getLeft() const;
			void setLeft(uint16_t value);

			uint16_t getTop() const;
			void setTop(uint16_t value);

			uint16_t getRight() const;
			void setRight(uint16_t value);

			uint16_t getBottom() const;
			void setBottom(uint16_t value);

			uint16_t getHorizontal() const;
			uint16_t getVertical() const;

			bool operator==(const Margin& rhs) const;
			bool operator!=(const Margin& rhs) const;

		private:
			uint16_t _left;
			uint16_t _top;
			uint16_t _right;
			uint16_t _bottom;
	};
}