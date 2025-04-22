#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/color.h"

namespace YOBA {
	class SecondaryColorElement : public virtual Element {
		public:
			const Color* getSecondaryColor() const {
				return _secondaryColor;
			}

			void setSecondaryColor(const Color* value) {
				if (value == _secondaryColor)
					return;

				_secondaryColor = value;

				invalidate();
			}

		private:
			const Color* _secondaryColor = nullptr;
	};
}
