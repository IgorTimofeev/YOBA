#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba::ui {
	class TextColorElement : public virtual Element {
		public:
			const Color* getTextColor() const {
				return _textColor;
			}

			void setTextColor(const Color* value) {
				_textColor = value;

				invalidate();
			}

		private:
			const Color* _textColor = nullptr;
	};
}
