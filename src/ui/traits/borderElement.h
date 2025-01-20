#pragma once

#include "ui/element.h"
#include "main/color.h"

namespace yoba::ui {
	class BorderElement : public virtual Element {
		public:
			const Color* getBorderColor() const {
				return _borderColor;
			}

			void setBorderColor(const Color* value) {
				if (value == _borderColor)
					return;

				_borderColor = value;

				invalidate();
			}

		private:
			const Color* _borderColor = nullptr;
	};
}
