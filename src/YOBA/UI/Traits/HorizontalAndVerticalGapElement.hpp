#pragma once

#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class HorizontalAndVerticalGapElement : public virtual Element {
		public:
			uint16_t getHorizontalGap() const {
				return _horizontalGap;
			}

			void setHorizontalGap(const uint16_t value) {
				if (value == _horizontalGap)
					return;

				_horizontalGap = value;

				invalidate();
			}

			uint16_t getVerticalGap() const {
				return _verticalGap;
			}

			void setVerticalGap(const uint16_t value) {
				if (value == _verticalGap)
					return;

				_verticalGap = value;

				invalidate();
			}

			void setGap(const uint16_t horizontal, const uint16_t vertical) {
				if (horizontal == _horizontalGap && vertical == _verticalGap)
					return;

				_horizontalGap = horizontal;
				_verticalGap = vertical;

				invalidate();
			}

			void setGap(const uint16_t uniformValue) {
				setGap(uniformValue, uniformValue);
			}

		private:
			int32_t _horizontalGap = 0;
			int32_t _verticalGap = 0;
	};
}
