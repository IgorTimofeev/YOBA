#include "touchDriver.h"

namespace yoba {
	volatile bool TouchDriver::_interruptFlag = false;

	void TouchDriver::onTouchInterrupted() {
		_interruptFlag = true;
	}

	volatile bool TouchDriver::getInterruptFlag() {
		return _interruptFlag;
	}

	void TouchDriver::clearInterruptFlag() {
		_interruptFlag = false;
	}
}