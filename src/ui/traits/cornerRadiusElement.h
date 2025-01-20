#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class CornerRadiusElement : public virtual Element {
		public:
			uint16_t getCornerRadius() const {
				return _cornerRadius;
			}

			void setCornerRadius(uint16_t value) {
				if (value == _cornerRadius)
					return;

				_cornerRadius = value;

				invalidate();
			}

		private:
			uint16_t _cornerRadius = 0;
	};
}
