#pragma once

#include <Wire.h>
#include "cstdlib"
#include "../touchPoint.h"
#include "FunctionalInterrupt.h"

namespace yoba {
	class TouchDriver {
		public:
			TouchDriver(uint8_t sdaPin, uint8_t sclPin, uint8_t rstPin, uint8_t intPin) : _sdaPin(sdaPin), _sclPin(sclPin), _rstPin(rstPin), _intPin(intPin) {

			}

			virtual void begin();

			virtual bool readPoint1Down() = 0;
			virtual bool readPoint2Down() = 0;

			virtual Point readPoint1() = 0;
			virtual Point readPoint2() = 0;

			volatile bool getInterruptFlag() const;
			void clearInterruptFlag();

		protected:
			uint8_t _sdaPin;
			uint8_t _sclPin;
			uint8_t _rstPin;
			uint8_t _intPin;

		private:
			volatile bool _interrupted = false;
			static void onInterrupt(TouchDriver* driver);
	};
}