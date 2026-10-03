#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Hardware/Displays/SPIDisplayInterface.hpp>

namespace YOBA {
	void SPIDisplayInterface::setup(
		const uint8_t MOSIPin,
		const uint8_t SCKPin,
		const int8_t SSPin,
		const int8_t DCPin,
		const int8_t RSTPin,

		const uint8_t bus,
		const uint8_t mode,
		const uint32_t frequencyHz
	) {
		_DCPin = DCPin;

		DisplayInterface::setup(
			RSTPin
		);

		_SPIDevice.setup(
			MOSIPin,
			SCKPin,
			SSPin,

			bus,
			mode,
			frequencyHz
		);

		// D/C pin
		if (_DCPin >= 0) {
			GPIO::setMode(_DCPin, GPIO::PinMode::output);
			GPIO::write(_DCPin, false);
		}
	}

	SPIDevice& SPIDisplayInterface::getSPIDevice() {
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
		GPIO::write(_DCPin, false);
		const auto result = _SPIDevice.write(command);
		GPIO::write(_DCPin, true);

		return result;
	}

	void SPIDisplayInterface::setDCPinState(const bool state) const {
		GPIO::write(_DCPin, state);
	}
}

#endif