#pragma once

#include <Wire.h>
#include "cstdlib"
#include "../touchPoint.h"

namespace yoba {
	class TouchDriver {
		public:
			TouchDriver(uint8_t sdaPin, uint8_t sclPin, uint8_t rstPin, uint8_t intPin) : _sdaPin(sdaPin), _sclPin(sclPin), _rstPin(rstPin), _intPin(intPin) {

			}

			virtual void begin() {
				// Interrupt
				pinMode(_intPin, INPUT_PULLUP);
				attachInterrupt(digitalPinToInterrupt(_intPin), onTouchInterrupted, CHANGE);

				// I2C
				Wire.begin(_sdaPin, _sclPin);

				// Reset Pin Configuration
				pinMode(_rstPin, OUTPUT);
				digitalWrite(_rstPin, LOW);
				delay(10);
				digitalWrite(_rstPin, HIGH);
			}

			virtual bool readPoint1Down() = 0;
			virtual bool readPoint2Down() = 0;

			virtual Point readPoint1() = 0;
			virtual Point readPoint2() = 0;

			volatile bool getInterruptFlag();
			void clearInterruptFlag();

		protected:
			uint8_t _sdaPin;
			uint8_t _sclPin;
			uint8_t _rstPin;
			uint8_t _intPin;

		private:
			static volatile bool _interruptFlag;
			static void onTouchInterrupted();
	};
}