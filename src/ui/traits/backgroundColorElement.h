#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba::ui {
	class BackgroundColorElement : public virtual Element {
		public:
			const Color* getBackgroundColor() const {
				return _backgroundColor;
			}

			void setBackgroundColor(const Color* value) {
				_backgroundColor = value;

				invalidate();
			}

		private:
			const Color* _backgroundColor = nullptr;
	};
}
