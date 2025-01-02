#pragma once

#include "element.h"
#include "../color.h"

namespace yoba {
	class PrimaryColorElement : public virtual Element {
		public:
			const Color* getPrimaryColor() const {
				return _primaryColor;
			}

			void setPrimaryColor(const Color* value) {
				_primaryColor = value;

				invalidate();
			}

		protected:
			const Color* _primaryColor = nullptr;
	};
}
