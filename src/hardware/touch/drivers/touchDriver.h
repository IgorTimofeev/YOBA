#pragma once

#include <Wire.h>
#include <vector>
#include "cstdlib"
#include "../touchPoint.h"
#include "FunctionalInterrupt.h"
#include "event.h"
#include "hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class TouchDriver {
		public:
			virtual void begin() = 0;

			virtual void tick(ScreenBuffer* screenBuffer, const std::function<void(Event&)>& callback) = 0;
	};
}