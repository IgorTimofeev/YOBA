#pragma once

#include "hardware/screen/buffers/screenBuffer.h"
#include "primaryColorElement.h"

namespace yoba {
	class Shape : public PrimaryColorElement {
		public:
			Shape() = default;

			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				return { 0, 0 };
			}
	};
}
