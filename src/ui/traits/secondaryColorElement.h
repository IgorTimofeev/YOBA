#pragma once

#include "ui/element.h"
#include "color.h"

namespace yoba::ui {
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
			const Color* selectColor(
				bool condition,
				const Color* defaultColor,
				const Color* conditionColor
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

				return nullptr;
			}

		private:
			const Color* _secondaryColor = nullptr;
	};
}
