#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Hardware/Displays/SPIDisplayInterface.hpp>

namespace YOBA {
	void SPIDisplayInterface::setup(
		const uint8_t busIndex,
		const uint8_t mode,

		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const uint32_t frequencyHz,

		const int8_t DCPin,
		const int8_t RSTPin
	) {
		_DCPin = DCPin;
		_RSTPin = RSTPin;

		_SPIDevice.setup(
			busIndex,
			mode,

			MOSIPin,
			SCKPin,
			SSPin,

			frequencyHz
		);

		// D/C pin
		system::GPIO::setMode(_DCPin, system::GPIO::PinMode::output);
		system::GPIO::write(_DCPin, true);

		// RST pin
		if (_RSTPin >= 0) {
			system::GPIO::setMode(_RSTPin, system::GPIO::PinMode::output);
			system::GPIO::write(_RSTPin, true);
		}
	}

	system::SPIDevice& SPIDisplayInterface::getSPIDevice() {
		return _SPIDevice;
	}

	int8_t SPIDisplayInterface::getDCPin() const {
		return _DCPin;
	}

	int8_t SPIDisplayInterface::getRSTPin() const {
		return _RSTPin;
	}

	bool SPIDisplayInterface::write(const uint8_t data) {
		return _SPIDevice.write(data);
	}

	bool SPIDisplayInterface::write(const std::span<const uint8_t> data) {
		return _SPIDevice.write(data);
	}

	bool SPIDisplayInterface::writeCommand(const uint8_t command) {
		system::GPIO::write(_DCPin, false);
		const auto result = _SPIDevice.write(command);
		system::GPIO::write(_DCPin, true);

		return result;
	}

	void SPIDisplayInterface::setDCPinState(const bool state) const {
		system::GPIO::write(_DCPin, state);
	}

	void SPIDisplayInterface::setRSTPinState(const bool state) const {
		system::GPIO::write(_RSTPin, state);
	}

	void SPIDisplayInterface::toggleRSTPin(const uint32_t delayAfterLowMs, const uint32_t delayAfterHighMs) const {
		setRSTPinState(false);
		system::delayMs(delayAfterLowMs);

		setRSTPinState(true);
		system::delayMs(delayAfterHighMs);
	}
}

#endif