#pragma once

#include <functional>
#include <cstdint>
#include <cmath>

#include <YOBA/hardware/HID.h>
#include <YOBA/main/events/event.h>

namespace YOBA {
	// -------------------------------- Value changed event --------------------------------

	class EncoderValueChangedEvent : public Event {
		public:
			explicit EncoderValueChangedEvent(int32_t deltaPerSecond);

			static uint16_t typeID;

			int32_t getDPS() const;

			template<typename TValue>
			TValue getDPSFactor(uint16_t check1, TValue value1, TValue valueElse);

			template<typename TValue>
			TValue getDPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse);

		private:
			int32_t _DPS;
	};

	template<typename TValue>
	TValue EncoderValueChangedEvent::getDPSFactor(uint16_t check1, TValue value1, TValue valueElse) {
		TValue value;

		if (std::abs(_DPS) < check1) {
			value = value1;
		}
		else {
			value = valueElse;
		}

		return _DPS >= 0 ? value : -value;
	}

	template<typename TValue>
	TValue EncoderValueChangedEvent::getDPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse) {
		const auto absRPS = std::abs(_DPS);
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

		return _DPS >= 0 ? value : -value;
	}

	// -------------------------------- Encoder --------------------------------

	class Encoder : public HID {
		public:
			Encoder(uint8_t aPin, uint8_t bPin);

			virtual void setup();
			void tick() override;

		private:
			uint8_t _aPin;
			uint8_t _bPin;
			uint8_t _oldAB = 0;
			uint64_t _oldValueTime = 0;
			int32_t _value = 0;

			static void abInterruptHandler(void* args);
			void readValue();
	};

	// -------------------------------- Push down event --------------------------------

	class PushButtonEncoderDownEvent : public Event {
		public:
			explicit PushButtonEncoderDownEvent();

			static uint16_t typeID;
	};

	// -------------------------------- Push up event --------------------------------

	class PushButtonEncoderUpEvent : public Event {
		public:
			explicit PushButtonEncoderUpEvent();

			static uint16_t typeID;
	};

	// -------------------------------- PushButtonEncoder --------------------------------

	class PushButtonEncoder : public Encoder {
		public:
			PushButtonEncoder(uint8_t aPin, uint8_t bPin, uint8_t swPin);

			void setup() override;
			void tick() override;

		private:
			uint8_t _swPin;

			bool _oldPressed = false;
			bool _pressed = false;

			void readPressed();
			static void swInterruptHandler(void* args);
	};
}
