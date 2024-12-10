#pragma once

#include "screenDriver.h"

namespace yoba {
	class ContrastScreenDriver : public virtual ScreenDriver {
		public:
			ContrastScreenDriver();

			virtual void setContrast(uint8_t value) = 0;
	};
}