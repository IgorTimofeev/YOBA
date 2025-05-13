#pragma once

#include "element.h"

namespace YOBA {
	class ModalElement : public virtual Element {
		public:
			void show();
			void hide();
	};
}