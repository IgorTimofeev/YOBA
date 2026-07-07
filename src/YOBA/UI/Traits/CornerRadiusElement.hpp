#pragma once

#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class CornerRadiusElement : public virtual Element {
		public:
			uint16_t getCornerRadius() const {
				return _cornerRadius;
			}

			void setCornerRadius(const uint16_t value) {
				if (value == _cornerRadius)
					return;

				_cornerRadius = value;

				invalidate();
			}

		private:
			uint16_t _cornerRadius = 0;
	};
}
