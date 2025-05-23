#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class HorizontalAndVerticalSpacingElement : public virtual Element {
		public:
			uint16_t getHorizontalSpacing() const {
				return _horizontalSpacing;
			}

			void setHorizontalSpacing(uint16_t value) {
				if (value == _horizontalSpacing)
					return;

				_horizontalSpacing = value;

				invalidate();
			}

			uint16_t getVerticalSpacing() const {
				return _verticalSpacing;
			}

			void setVerticalSpacing(uint16_t value) {
				if (value == _verticalSpacing)
					return;

				_verticalSpacing = value;

				invalidate();
			}

			void setSpacing(uint16_t horizontal, uint16_t vertical) {
				if (horizontal == _horizontalSpacing && vertical == _verticalSpacing)
					return;

				_horizontalSpacing = horizontal;
				_verticalSpacing = vertical;

				invalidate();
			}

			void setSpacing(uint16_t uniformValue) {
				setSpacing(uniformValue, uniformValue);
			}

		private:
			int32_t _horizontalSpacing = 0;
			int32_t _verticalSpacing = 0;
	};
}
