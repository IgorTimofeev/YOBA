#pragma once

#include "container.h"
#include "rectangle.h"

namespace yoba {
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