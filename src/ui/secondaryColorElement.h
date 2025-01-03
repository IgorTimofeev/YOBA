#pragma once

#include "element.h"
#include "../color.h"

namespace yoba {
	class SecondaryColorElement : public virtual Element {
		public:
			const Color* getSecondaryColor() const {
				return _secondaryColor;
			}

			void setSecondaryColor(const Color* value) {
				_secondaryColor = value;

				invalidate();
			}

		protected:
			const Color* _secondaryColor = nullptr;

			const Color* selectColor(
				bool condition,
				const Color* defaultColor,
				const Color* conditionColor,
				const Color* fallbackColor
			) {
				if (condition) {
					if (conditionColor) {
						return conditionColor;
					}
					else if (defaultColor) {
						return defaultColor;
					}
				}
				else if (defaultColor) {
					return defaultColor;
				}

				return fallbackColor;
			}
	};
}
