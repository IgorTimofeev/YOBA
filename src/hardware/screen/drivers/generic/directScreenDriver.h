#pragma once

#include "screenDriver.h"

namespace yoba {
	class DirectScreenDriver : public virtual ScreenDriver {
		public:
			DirectScreenDriver();

			virtual void writePixels(uint8_t* source) = 0;
	};
}