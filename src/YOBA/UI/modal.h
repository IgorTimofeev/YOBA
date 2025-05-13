#pragma once

#include "element.h"

namespace YOBA {
	class ModalElement : public virtual Element {
		public:
			void show(Application* application);
			void hide();
	};
}