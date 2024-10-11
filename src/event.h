#pragma once

#include "point.h"

namespace yoba {
	// -------------------------------- Event --------------------------------

	class Element;

	class Event {
		public:
			virtual bool matches(Element* element);

			bool isHandled() const;
			void setHandled(bool handled);

		private:
			bool _handled = false;
	};

	// -------------------------------- InputEvent --------------------------------

	class InputEvent : public Event {
		public:

	};

	class ScreenEvent : public InputEvent {
		public:
			bool matches(Element* element) override;
	};

	class TouchEvent : public ScreenEvent {
		public:
			explicit TouchEvent(const Point& position);

			bool matches(Element* element) override;

			const Point& getPosition() const;
			void setPosition(const Point& position);

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
			explicit PinchEvent(const Point& position1, const Point& position2);

			bool matches(Element* element) override;

			const Point& getPosition1() const;
			void setPosition1(const Point& position1);

			const Point& getPosition2() const;
			void setPosition2(const Point& position2);

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

	// -------------------------------- ElementEvent --------------------------------

	template<typename TTarget>
	class TargetEvent : public Event {
		public:
			explicit TargetEvent(TTarget target);
			TTarget getTarget() const;

		private:
			TTarget _target;
	};

	template<typename TElement>
	TElement TargetEvent<TElement>::getTarget() const {
		return _target;
	}

	template<typename TElement>
	TargetEvent<TElement>::TargetEvent(TElement target) : _target(target) {

	}
}