#pragma once

#include "ui/element.h"
#include "main/color.h"

namespace yoba::ui {
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
