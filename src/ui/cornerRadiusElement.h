#pragma once

#include "element.h"

namespace yoba {
	class CornerRadiusElement : public virtual Element {
		public:
			uint16_t getCornerRadius() const {
				return _cornerRadius;
			}

			void setCornerRadius(uint16_t value) {
				_cornerRadius = value;

				invalidate();
			}

		protected:
			uint16_t _cornerRadius = 0;
	};
}
