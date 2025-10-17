#include "encoder.h"

#include <YOBA/system.h>
#include <YOBA/UI/application.h>

namespace YOBA {
	// -------------------------------- Value changed event --------------------------------

	EncoderValueChangedEvent::EncoderValueChangedEvent(int16_t deltaPerSecond) : Event(typeID), _DPS(deltaPerSecond) {

	}

	uint16_t EncoderValueChangedEvent::typeID = registerTypeID();

	int16_t EncoderValueChangedEvent::getDPS() const {
		return _DPS;
	}

	// -------------------------------- Encoder --------------------------------

	Encoder::Encoder(uint8_t aPin, uint8_t bPin) :
		_aPin(aPin),
		_bPin(bPin)
	{

	}

	void Encoder::setup() {
		system::GPIO::setMode(_aPin, system::GPIO::PinMode::input);
		system::GPIO::setMode(_bPin, system::GPIO::PinMode::input);

		system::GPIO::addInterruptHandler(_aPin, abInterruptHandler, this);
		system::GPIO::addInterruptHandler(_bPin, abInterruptHandler, this);
	}

	void Encoder::tick() {
		if (std::abs(_value) < _minimumDelta)
			return;

		const auto application = Application::getCurrent();

		const auto time = system::getTime();
		const auto deltaTime = static_cast<int32_t>(time - _oldValueTime);
		_oldValueTime = time;

		const auto dps = static_cast<int16_t>(static_cast<int32_t>(_value) * static_cast<int32_t>(1'000'000) / deltaTime);

		auto event = EncoderValueChangedEvent(dps);
		application->pushEvent(&event);

		_value = 0;
	}

	uint16_t Encoder::getMinimumDelta() const {
		return _minimumDelta;
	}

	void Encoder::setMinimumDelta(uint16_t value) {
		_minimumDelta = value;
	}

	void Encoder::abInterruptHandler(void* args) {
		static_cast<Encoder*>(args)->readValue();
	}

	void Encoder::readValue() {
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
					_value++;
					break;
				case 2:
				case 4:
				case 11:
				case 13:
					_value--;
					break;
				case 3:
				case 12:
					_value += 2;
					break;
				default:
					_value -= 2;
					break;
			}

			_oldAB = AB;
		}
	}

	// -------------------------------- Push down event --------------------------------

	PushButtonEncoderDownEvent::PushButtonEncoderDownEvent() : Event(typeID) {

	}

	uint16_t PushButtonEncoderDownEvent::typeID = registerTypeID();

	// -------------------------------- Push up event --------------------------------

	PushButtonEncoderUpEvent::PushButtonEncoderUpEvent() : Event(typeID) {

	}

	uint16_t PushButtonEncoderUpEvent::typeID = registerTypeID();


	// -------------------------------- PushButtonEncoder --------------------------------

	PushButtonEncoder::PushButtonEncoder(uint8_t aPin, uint8_t bPin, uint8_t swPin): Encoder(aPin, bPin), _swPin(swPin) {

	}

	void PushButtonEncoder::setup() {
		Encoder::setup();

		system::GPIO::setMode(_swPin, system::GPIO::PinMode::input);
		system::GPIO::addInterruptHandler(_swPin, swInterruptHandler, this);
		readPressed();
	}

	void PushButtonEncoder::tick() {
		Encoder::tick();

		if (_pressed == _oldPressed)
			return;

		const auto application = Application::getCurrent();

		if (_pressed) {
			auto event = PushButtonEncoderDownEvent();
			application->pushEvent(&event);
		}
		else {
			auto event = PushButtonEncoderUpEvent();
			application->pushEvent(&event);
		}

		_oldPressed = _pressed;
	}

	void PushButtonEncoder::swInterruptHandler(void* args) {
		static_cast<PushButtonEncoder*>(args)->readPressed();
	}

	void PushButtonEncoder::readPressed() {
		_pressed = !system::GPIO::read(_swPin);
	}

	bool PushButtonEncoder::isPressed() const {
		return _pressed;
	}
}
