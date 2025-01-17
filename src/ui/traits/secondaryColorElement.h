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

		private:
			const Color* _secondaryColor = nullptr;
	};
}
