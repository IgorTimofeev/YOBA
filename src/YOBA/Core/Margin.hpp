#pragma once

#include <cstdint>

namespace YOBA {
	class Margin {
		public:
			constexpr Margin(const int32_t left, const int32_t top, const int32_t right, const int32_t bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {

			}

			constexpr Margin(const int32_t horizontal, const int32_t vertical) : _left(horizontal), _top(vertical), _right(horizontal), _bottom(vertical) {

			}

			constexpr Margin(const int32_t uniformValue) : _left(uniformValue), _top(uniformValue), _right(uniformValue), _bottom(uniformValue) {

			}

			constexpr Margin() : _left(0), _top(0), _right(0), _bottom(0) {

			}

			static Margin zero;

			int32_t getLeft() const;
			void setLeft(int32_t value);

			int32_t getTop() const;
			void setTop(int32_t value);

			int32_t getRight() const;
			void setRight(int32_t value);

			int32_t getBottom() const;
			void setBottom(int32_t value);

			int32_t getHorizontal() const;
			int32_t getVertical() const;

			bool operator==(const Margin& rhs) const;
			bool operator!=(const Margin& rhs) const;

		private:
			int32_t _left;
			int32_t _top;
			int32_t _right;
			int32_t _bottom;
	};
}