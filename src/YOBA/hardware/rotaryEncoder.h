#pragma once

#include <YOBA/hardware/HID.h>
#include <YOBA/main/event.h>
#include <functional>

namespace YOBA {
	// -------------------------------- Rotation event --------------------------------

	class RotaryEncoderRotationEvent : public Event {
		public:
			explicit RotaryEncoderRotationEvent(int32_t rps);

			static uint16_t typeID;

			int32_t getRPS() const;

			template<typename TValue>
			TValue getRPSFactor(uint16_t check1, TValue value1, TValue valueElse);

			template<typename TValue>
			TValue getRPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse);

		private:
			int32_t _RPS;
	};

	template<typename TValue>
	TValue RotaryEncoderRotationEvent::getRPSFactor(uint16_t check1, TValue value1, TValue valueElse) {
		TValue value;

		if (std::abs(_RPS) < check1) {
			value = value1;
		}
		else {
			value = valueElse;
		}

		return _RPS >= 0 ? value : -value;
	}

	template<typename TValue>
	TValue RotaryEncoderRotationEvent::getRPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse) {
		const auto absRPS = std::abs(_RPS);
		TValue value;

		if (absRPS < check1) {
			value = value1;
		}
		else if (absRPS < check2) {
			value = value2;
		}
		else {
			value = valueElse;
		}

		return _RPS >= 0 ? value : -value;
	}

	// -------------------------------- Switch event --------------------------------

	class RotaryEncoderSwitchEvent : public Event {
		public:
			explicit RotaryEncoderSwitchEvent(bool pressed);

			static uint16_t typeID;

			bool isPressed() const;

		private:
			bool _pressed;
	};

	// -------------------------------- Encoder --------------------------------

	class RotaryEncoder : public HID {
		public:
			RotaryEncoder(uint8_t aPin, uint8_t bPin, uint8_t swPin);

			void setup();
			void tick() override;

		private:
			uint8_t _aPin;
			uint8_t _bPin;
			uint8_t _swPin;

			bool _interrupted = false;
			uint8_t _oldAB = 0;

			uint64_t _previousRotationTime = 0;
			int32_t _rotation = 0;

			bool _previousPressed = false;
			bool _pressed = false;

			static void abInterruptHandler(void* args);
			static void swInterruptHandler(void* args);
			void readRotation();
			void readPressed();
	};
}
