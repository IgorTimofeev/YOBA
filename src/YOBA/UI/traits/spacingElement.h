#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class SpacingElement : public virtual Element {
		public:
			int32_t getSpacing() const {
				return _spacing;
			}

			void setSpacing(const int32_t value) {
				if (value == _spacing)
					return;

				_spacing = value;

				invalidate();
			}

		private:
			int32_t _spacing = 0;
	};
}
