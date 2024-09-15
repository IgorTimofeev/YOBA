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
			TouchDriver(uint8_t sdaPin, uint8_t sclPin, uint8_t rstPin, uint8_t intPin) : _sdaPin(sdaPin), _sclPin(sclPin), _rstPin(rstPin), _intPin(intPin) {

			}

			virtual void begin();

			virtual bool readPoint1Down() = 0;
			virtual bool readPoint2Down() = 0;

			virtual Point readPoint1() = 0;
			virtual Point readPoint2() = 0;

			void tick(ScreenBuffer* screenBuffer, const std::function<void(Event&)>& callback);

		protected:
			uint8_t _sdaPin;
			uint8_t _sclPin;
			uint8_t _rstPin;
			uint8_t _intPin;

			bool _isTouched = false;
			bool _isPinched = false;

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			static Point rotatePoint(ScreenBuffer* screenBuffer, Point point);
			Point readRotatedPoint1(ScreenBuffer* screenBuffer);
			Point readRotatedPoint2(ScreenBuffer* screenBuffer);

			volatile bool _interrupted = false;
			static void onInterrupt(TouchDriver* driver);
	};
}