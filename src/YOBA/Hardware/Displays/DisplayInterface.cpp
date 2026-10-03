#include <YOBA/Hardware/Displays/DisplayInterface.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void DisplayInterface::setup(const int8_t RSTPin) {
		_RSTPin = RSTPin;

		// RST pin
		if (_RSTPin >= 0) {
			GPIO::setMode(_RSTPin, GPIO::PinMode::output);
			GPIO::write(_RSTPin, true);
		}
	}

	int8_t DisplayInterface::getRSTPin() const {
		return _RSTPin;
	}

	void DisplayInterface::setRSTPinState(const bool state) const {
		GPIO::write(_RSTPin, state);
	}

	void DisplayInterface::toggleRSTPin(const uint32_t delayAfterLowMs, const uint32_t delayAfterHighMs) const {
		setRSTPinState(false);
		Timer::delayMs(delayAfterLowMs);

		setRSTPinState(true);
		Timer::delayMs(delayAfterHighMs);
	}
}
