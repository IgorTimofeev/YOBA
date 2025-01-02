#pragma once

#include "element.h"
#include "../color.h"

namespace yoba {
	class BorderElement : public virtual Element {
		public:
			const Color* getBorderColor() const {
				return _borderColor;
			}

			void setBorderColor(const Color* value) {
				_borderColor = value;

				invalidate();
			}

		protected:
			const Color* _borderColor = nullptr;
	};
}
