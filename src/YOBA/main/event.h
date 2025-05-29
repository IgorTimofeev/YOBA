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
			virtual ~Event() = default;

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

			static bool isScreen(const Event* event);
	};

	// -------------------------------- Pointer --------------------------------

	class PointerEvent : public ScreenEvent {
		public:
			PointerEvent(uint16_t& staticTypeID, const Point& position);

			const Point& getPosition() const;
			void setPosition(const Point& position);

			static bool isPointer(const Event* event);

		private:
			Point _position;
	};

	class PointerDownEvent : public PointerEvent {
		public:
			explicit PointerDownEvent(const Point& position);

			static uint16_t typeID;
	};

	class PointerDragEvent : public PointerEvent {
		public:
			explicit PointerDragEvent(const Point& position);

			static uint16_t typeID;
	};

	class PointerUpEvent : public PointerEvent {
		public:
			explicit PointerUpEvent(const Point& position);

			static uint16_t typeID;
	};

	class PinchEvent : public ScreenEvent {
		public:
			PinchEvent(uint16_t& staticTypeID, const Point& position1, const Point& position2);

			const Point& getPosition1() const;
			void setPosition1(const Point& position1);

			const Point& getPosition2() const;
			void setPosition2(const Point& position2);

			float getLength() const;

			static bool isPinch(const Event* event);

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