#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba {
	class ForegroundColorAware : public virtual Element {
		public:
			const Color* getForegroundColor() const {
				return _foregroundColor;
			}

			void setForegroundColor(const Color* value) {
				_foregroundColor = value;

				invalidate();
			}

		protected:
			const Color* _foregroundColor = nullptr;
	};
}
