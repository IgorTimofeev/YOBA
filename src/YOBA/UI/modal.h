#pragma once

#include "layout.h"
#include "YOBA/UI/shapes/rectangle.h"

namespace YOBA {
	class ModalLayout : public Layout {
		public:
			void show(Application* application);
			void hide();
	};
}