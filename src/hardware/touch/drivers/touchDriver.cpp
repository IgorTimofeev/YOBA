#include "touchDriver.h"

namespace yoba {
	void TouchDriver::begin() {
		// Interrupt
		pinMode(_intPin, INPUT_PULLUP);

		attachInterrupt(
			digitalPinToInterrupt(_intPin),
			std::bind(&onInterrupt, this),
			CHANGE
		);

		// I2C
		Wire.begin(_sdaPin, _sclPin);

		// Reset Pin Configuration
		pinMode(_rstPin, OUTPUT);
		digitalWrite(_rstPin, LOW);
		delay(10);
		digitalWrite(_rstPin, HIGH);
	}

	void TouchDriver::onInterrupt(TouchDriver* driver) {
		driver->_interrupted = true;
	}

	volatile bool TouchDriver::getInterruptFlag() const {
		return _interrupted;
	}

	void TouchDriver::clearInterruptFlag() {
		_interrupted = false;
	}
}