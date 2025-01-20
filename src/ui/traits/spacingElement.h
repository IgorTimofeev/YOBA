#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class SpacingElement : public virtual Element {
		public:
			uint16_t getSpacing() const {
				return _spacing;
			}

			void setSpacing(uint16_t value) {
				if (value == _spacing)
					return;

				_spacing = value;

				invalidate();
			}

		private:
			uint16_t _spacing = 0;
	};
}
