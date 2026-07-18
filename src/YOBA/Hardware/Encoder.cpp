#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_GPIO)

#include <YOBA/Hardware/Encoder.hpp>
#include <YOBA/System.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	// -------------------------------- Value changed event --------------------------------

	EncoderValueChangedEvent::EncoderValueChangedEvent(const int16_t deltaPerSecond) : Event(typeID), _DPS(deltaPerSecond) {

	}

	uint16_t EncoderValueChangedEvent::typeID = registerTypeID();

	int16_t EncoderValueChangedEvent::getDPS() const {
		return _DPS;
	}

	// -------------------------------- Encoder --------------------------------

	Encoder::Encoder(const uint8_t aPin, const uint8_t bPin) :
		_aPin(aPin),
		_bPin(bPin)
	{

	}

	void Encoder::setup() {
		system::GPIO::setMode(_aPin, system::GPIO::PinMode::input);
		system::GPIO::setMode(_bPin, system::GPIO::PinMode::input);

		system::GPIO::addInterruptHandler(_aPin, ABInterruptHandler, this);
		system::GPIO::addInterruptHandler(_bPin, ABInterruptHandler, this);
	}

	void Encoder::tick() {
		if (std::abs(_value) < _minimumDelta)
			return;

		const auto application = Application::getCurrent();

		const auto time = system::getTimeUs();
		const auto deltaTime = static_cast<int32_t>(time - _oldValueTime);
		_oldValueTime = time;

		const auto dps = static_cast<int16_t>(static_cast<int32_t>(_value) * static_cast<int32_t>(1'000'000) / deltaTime);

		EncoderValueChangedEvent event { dps };
		application->pushEvent(&event);

		_value = 0;
	}

	uint16_t Encoder::getMinimumDelta() const {
		return _minimumDelta;
	}

	void Encoder::setMinimumDelta(const uint16_t value) {
		_minimumDelta = value;
	}

	void Encoder::ABInterruptHandler(void* arg) {
		static_cast<Encoder*>(arg)->readAB();
	}

	void Encoder::readAB() {
		const auto AB = system::GPIO::read(_aPin) << 1 | system::GPIO::read(_bPin);

		switch (_oldAB | AB << 2) {
			// Clockwise
			case 0b0010:
			case 0b0100:
			case 0b1011:
			case 0b1101:
				_value++;
				break;

			// Counterclockwise
			case 0b0001:
			case 0b0111:
			case 0b1000:
			case 0b1110:
				_value--;
				break;

			default:
				break;
		}

		_oldAB = AB;
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

	PushButtonEncoder::PushButtonEncoder(const uint8_t aPin, const uint8_t bPin, const uint8_t swPin): Encoder(aPin, bPin), _swPin(swPin) {

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
			PushButtonEncoderDownEvent event {};
			application->pushEvent(&event);
		}
		else {
			PushButtonEncoderUpEvent event {};
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

#endif