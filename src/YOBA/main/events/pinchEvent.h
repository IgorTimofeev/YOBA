#pragma once

#include <cstdint>

#include <YOBA/main/events/screenEvent.h>
#include <YOBA/main/vector2.h>

namespace YOBA {
	class PinchEvent : public ScreenEvent {
		public:
			PinchEvent(uint16_t typeID, const Point& position1, const Point& position2);

			const Point& getPosition1() const;
			void setPosition1(const Point& position1);

			const Point& getPosition2() const;
			void setPosition2(const Point& position2);

			float getLength() const;

			static bool isPinch(const Event& event);

		private:
			Point _position1, _position2;
	};

	class PinchDownEvent : public PinchEvent {
		public:
			PinchDownEvent(const Point& position1, const Point& position2);

			static uint16_t typeID;
	};

	class PinchDragEvent : public PinchEvent {
		public:
			PinchDragEvent(const Point& position1, const Point& position2);

			static uint16_t typeID;
	};

	class PinchUpEvent : public PinchEvent {
		public:
			PinchUpEvent(const Point& position1, const Point& position2);

			static uint16_t typeID;
	};
}