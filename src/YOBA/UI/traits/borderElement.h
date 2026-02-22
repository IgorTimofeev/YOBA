#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/color.h>

namespace YOBA {
	class BorderElement : public virtual Element {
		public:
			bool hasBorderColor() const {
				return _borderColor;
			}

			const Color& getBorderColor() const {
				return *_borderColor;
			}

			void setBorderColor(const Color& value) {
				if (&value == _borderColor)
					return;

				_borderColor = &value;

				invalidate();
			}

		private:
			const Color* _borderColor = nullptr;
	};
}
