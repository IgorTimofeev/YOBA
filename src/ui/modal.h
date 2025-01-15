#pragma once

#include "layout.h"
#include "ui/shapes/rectangle.h"

namespace yoba::ui {
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