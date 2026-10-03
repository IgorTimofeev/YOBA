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

		DisplayInterface::setup(
			RSTPin
		);

		_SPIDevice.setup(
			busIndex,
			mode,

			MOSIPin,
			SCKPin,
			SSPin,

			frequencyHz
		);

		// D/C pin
		if (_DCPin >= 0) {
			system::GPIO::setMode(_DCPin, system::GPIO::PinMode::output);
			system::GPIO::write(_DCPin, false);
		}
	}

	system::SPIDevice& SPIDisplayInterface::getSPIDevice() {
		return _SPIDevice;
	}

	int8_t SPIDisplayInterface::getDCPin() const {
		return _DCPin;
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
}

#endif