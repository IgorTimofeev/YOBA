#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba {
	class BackgroundColorAware : public virtual Element {
		public:
			const Color* getBackgroundColor() const {
				return _backgroundColor;
			}

			void setBackgroundColor(const Color* value) {
				_backgroundColor = value;

				invalidate();
			}

		protected:
			const Color* _backgroundColor = nullptr;
	};
}
