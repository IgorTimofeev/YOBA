#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba::ui {
	class PrimaryColorElement : public virtual Element {
		public:
			const Color* getPrimaryColor() const {
				return _primaryColor;
			}

			void setPrimaryColor(const Color* value) {
				_primaryColor = value;

				invalidate();
			}

		private:
			const Color* _primaryColor = nullptr;
	};
}
