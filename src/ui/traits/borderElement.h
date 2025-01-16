#pragma once

#include "ui/element.h"
#include "color.h"

namespace yoba::ui {
	class BorderElement : public virtual Element {
		public:
			const Color* getBorderColor() const {
				return _borderColor;
			}

			void setBorderColor(const Color* value) {
				_borderColor = value;

				invalidate();
			}

		private:
			const Color* _borderColor = nullptr;
	};
}
