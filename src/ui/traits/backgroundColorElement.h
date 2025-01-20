#pragma once

#include "../element.h"
#include "main/color.h"

namespace yoba::ui {
	class BackgroundColorElement : public virtual Element {
		public:
			const Color* getBackgroundColor() const {
				return _backgroundColor;
			}

			void setBackgroundColor(const Color* value) {
				if (value == _backgroundColor)
					return;

				_backgroundColor = value;

				invalidate();
			}

		private:
			const Color* _backgroundColor = nullptr;
	};
}
