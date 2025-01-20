#pragma once

#include "../element.h"
#include "main/color.h"

namespace yoba::ui {
	class FillColorElement : public virtual Element {
		public:
			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				if (value == _fillColor)
					return;

				_fillColor = value;

				invalidate();
			}

		private:
			const Color* _fillColor = nullptr;
	};
}
