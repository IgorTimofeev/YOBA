#pragma once

#include <cstdint>

namespace YOBA {
	class Margin {
		public:
			Margin() = default;
			explicit Margin(int32_t uniformValue);
			Margin(int32_t horizontal, int32_t vertical);
			Margin(int32_t left, int32_t top, int32_t right, int32_t bottom);

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
			int32_t left = 0;
			int32_t top = 0;
			int32_t right = 0;
			int32_t bottom = 0;
	};
}