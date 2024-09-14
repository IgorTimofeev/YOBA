#pragma once

#include "point.h"

namespace yoba {
	enum EventType {
		touchDown,
		touchDrag,
		touchUp,

		pinchDown,
		pinchDrag,
		pinchUp,
	};

	class Element;

	class Event {
		public:
			explicit Event(EventType type);

			EventType getType() const;

			virtual bool matches(Element* element) = 0;

			bool isHandled() const;

			void setHandled(bool handled);

		private:
			bool _handled = false;
			EventType _type;
	};

	class ScreenEvent : public Event {
		public:
			explicit ScreenEvent(EventType type);

			bool matches(Element *element) override;
	};

	class TouchEvent : public ScreenEvent {
		public:
			explicit TouchEvent(const EventType& type, const Point& position);

			const Point &getPosition() const;

			void setPosition(const Point &position);

			bool matches(Element* element) override;

		private:
			Point _position;
	};

	class TouchDownEvent : public TouchEvent {
		public:
			explicit TouchDownEvent(const Point& position);
	};

	class TouchDragEvent : public TouchEvent {
		public:
			explicit TouchDragEvent(const Point& position);
	};

	class TouchUpEvent : public TouchEvent {
		public:
			explicit TouchUpEvent(const Point& position);
	};

	class PinchEvent : public ScreenEvent {
		public:
			explicit PinchEvent(const EventType& type, const Point& position1, const Point& position2);

			bool matches(Element* element) override;

			const Point &getPosition1() const;

			void setPosition1(const Point &position1);

			const Point &getPosition2() const;

			void setPosition2(const Point &position2);

		private:
			Point _position1, _position2;
	};

	class PinchDownEvent : public PinchEvent {
		public:
			explicit PinchDownEvent(const Point& position1, const Point& position2);
	};

	class PinchDragEvent : public PinchEvent {
		public:
			explicit PinchDragEvent(const Point& position1, const Point& position2);
	};

	class PinchUpEvent : public PinchEvent {
		public:
			explicit PinchUpEvent(const Point& position1, const Point& position2);
	};
}