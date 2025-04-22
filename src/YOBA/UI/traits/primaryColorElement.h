#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/color.h"

namespace YOBA {
	class PrimaryColorElement : public virtual Element {
		public:
			const Color* getPrimaryColor() const {
				return _primaryColor;
			}

			void setPrimaryColor(const Color* value) {
				if (value == _primaryColor)
					return;

				_primaryColor = value;

				invalidate();
			}

		private:
			const Color* _primaryColor = nullptr;
	};
}
