#pragma once

#include "layout.h"
#include "ui/shapes/rectangle.h"

namespace yoba::ui {
	class ModalLayout : public Layout {
		public:
			void show(Application* application);
			void hide();
	};
}