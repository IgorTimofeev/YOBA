#include "rotaryEncoder.h"

#include <YOBA/system.h>
#include <YOBA/UI/application.h>

namespace YOBA {
	// -------------------------------- Rotation event --------------------------------

	uint16_t RotaryEncoderRotationEvent::typeID = 0;

	RotaryEncoderRotationEvent::RotaryEncoderRotationEvent(int32_t rps) : Event(typeID), _RPS(rps) {

	}

	int32_t RotaryEncoderRotationEvent::getRPS() const {
		return _RPS;
	}

	// -------------------------------- Switch event --------------------------------

	uint16_t RotaryEncoderSwitchEvent::typeID = 0;

	RotaryEncoderSwitchEvent::RotaryEncoderSwitchEvent(bool pressed) : Event(typeID), _pressed(pressed) {

	}

	bool RotaryEncoderSwitchEvent::isPressed() const {
		return _pressed;
	}

	// -------------------------------- Encoder --------------------------------

	RotaryEncoder::RotaryEncoder(uint8_t aPin, uint8_t bPin, uint8_t swPin) :
		_aPin(aPin),
		_bPin(bPin),
		_swPin(swPin) {
	}

	void RotaryEncoder::setup() {
		// GPIO
		system::GPIO::setMode(_aPin, system::GPIO::PinMode::input);
		system::GPIO::setMode(_bPin, system::GPIO::PinMode::input);
		system::GPIO::setMode(_swPin, system::GPIO::PinMode::input);

		system::GPIO::addInterruptHandler(_aPin, abInterruptHandler, this);
		system::GPIO::addInterruptHandler(_bPin, abInterruptHandler, this);
		system::GPIO::addInterruptHandler(_swPin, swInterruptHandler, this);
	}

	void RotaryEncoder::tick() {
		if (!_interrupted)
			return;

		_interrupted = false;

		const auto application = Application::getCurrent();

		// Rotation
		if (std::abs(_rotation) > 2) {
			const auto time = system::getTime();
			const auto deltaTime = time - _previousRotationTime;
			_previousRotationTime = time;

			const int32_t rps = _rotation * static_cast<int32_t>(1'000'000) / static_cast<int32_t>(deltaTime);

			auto event = RotaryEncoderRotationEvent(rps);
			application->handleEvent(&event);

			_rotation = 0;
		}

		// Push
		if (_pressed != _previousPressed) {
			auto event = RotaryEncoderSwitchEvent(_pressed);
			application->handleEvent(&event);

			_previousPressed = _pressed;
		}
	}

	void RotaryEncoder::abInterruptHandler(void* args) {
		const auto instance = static_cast<RotaryEncoder*>(args);

		instance->_interrupted = true;
		instance->readRotation();
	}

	void RotaryEncoder::swInterruptHandler(void* args) {
		const auto instance = static_cast<RotaryEncoder*>(args);

		instance->_interrupted = true;
		instance->readPressed();
	}

	void RotaryEncoder::readPressed() {
		_pressed = !system::GPIO::read(_swPin);
	}

	void RotaryEncoder::readRotation() {
		const auto AB = system::GPIO::read(_aPin) << 1 | system::GPIO::read(_bPin);

		if (AB != _oldAB) {
			switch (_oldAB | AB << 2) {
				case 0:
				case 5:
				case 10:
				case 15:
					break;
				case 1:
				case 7:
				case 8:
				case 14:
					_rotation++;
					break;
				case 2:
				case 4:
				case 11:
				case 13:
					_rotation--;
					break;
				case 3:
				case 12:
					_rotation += 2;
					break;
				default:
					_rotation -= 2;
					break;
			}

			_oldAB = AB;
		}
	}
}
