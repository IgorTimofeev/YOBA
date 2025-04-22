#pragma once


#include <vector>
#include <cstdint>
#include "vector2.h"

namespace YOBA {
	class Element;
}

namespace YOBA {
	// -------------------------------- Event --------------------------------

	class Event {
		public:
			explicit Event(uint16_t& staticTypeID);

			virtual bool matches(Element* element);

			bool isHandled() const;
			void setHandled(bool handled);
			uint16_t getTypeID() const;

		private:
			static uint16_t _nextTypeID;

			uint16_t _typeID = 0;
			bool _handled = false;
	};

	// -------------------------------- InputEvent --------------------------------

	class InputEvent : public Event {
		public:
			explicit InputEvent(uint16_t& staticTypeID);
	};

	// -------------------------------- Screen --------------------------------

	class ScreenEvent : public InputEvent {
		public:
			explicit ScreenEvent(uint16_t& staticTypeID);

			bool matches(Element* element) override;

			static bool isScreen(Event* event);
	};

	// -------------------------------- Touch --------------------------------

	class TouchEvent : public ScreenEvent {
		public:
			TouchEvent(uint16_t& staticTypeID, const Point& position);

			bool matches(Element* element) override;

			const Point& getPosition() const;
			void setPosition(const Point& position);

		private:
			Point _position;
	};

	class TouchDownEvent : public TouchEvent {
		public:
			explicit TouchDownEvent(const Point& position);

			static uint16_t typeID;
	};

	class TouchDragEvent : public TouchEvent {
		public:
			explicit TouchDragEvent(const Point& position);

			static uint16_t typeID;
	};

	class TouchUpEvent : public TouchEvent {
		public:
			explicit TouchUpEvent(const Point& position);

			static uint16_t typeID;
	};

	class PinchEvent : public ScreenEvent {
		public:
			PinchEvent(uint16_t& staticTypeID, const Point& position1, const Point& position2);

			bool matches(Element* element) override;

			const Point& getPosition1() const;
			void setPosition1(const Point& position1);

			const Point& getPosition2() const;
			void setPosition2(const Point& position2);

			float getLength() const;

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