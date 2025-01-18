#pragma once

#include "../element.h"
#include "../../color.h"

namespace yoba::ui {
	class FillColorElement : public virtual Element {
		public:
			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				_fillColor = value;

				invalidate();
			}

		private:
			const Color* _fillColor = nullptr;
	};
}
