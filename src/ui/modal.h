#pragma once

#include "container.h"
#include "ui/shapes/rectangle.h"

namespace yoba::ui {
	class ModalOverlay : public Container {
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