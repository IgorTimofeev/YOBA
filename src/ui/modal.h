#pragma once

#include "layout.h"
#include "rectangle.h"

namespace yoba {
	class ModalOverlay : public Layout {
		public:
			ModalOverlay() {
				*this += &_backgroundRectangle;
			}

			Rectangle& getBackgroundRectangle() {
				return _backgroundRectangle;
			}

		private:
			Rectangle _backgroundRectangle = Rectangle();
	};
}