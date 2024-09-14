#pragma once

#include "cstdlib"
#include "../touchPoint.h"

namespace yoba {
	class TouchDriver {
		public:
			TouchDriver(uint8_t sdaPin, uint8_t sclPin, uint8_t rstPin, uint8_t intPin) : _sdaPin(sdaPin), _sclPin(sclPin), _rstPin(rstPin), _intPin(intPin) {

			}

			virtual void begin() = 0;

			virtual bool isPoint1Down() = 0;
			virtual bool isPoint2Down() = 0;

			virtual Point readPoint1() = 0;
			virtual Point readPoint2() = 0;

		protected:
			uint8_t _sdaPin;
			uint8_t _sclPin;
			uint8_t _rstPin;
			uint8_t _intPin;
	};
}