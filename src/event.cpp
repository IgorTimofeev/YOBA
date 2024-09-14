#include "event.h"
#include "ui/element.h"

namespace yoba {
	Event::Event(EventType type) : _type(type) {

	}

	EventType Event::getType() const {
		return _type;
	}

	bool Event::isHandled() const {
		return _handled;
	}

	void Event::setHandled(bool handled) {
		_handled = handled;
	}

	bool ScreenEvent::matches(Element *element) {
		return element->isVisible() && element->isEnabled();
	}

	TouchEvent::TouchEvent(const EventType &type, const Point &position) :
		ScreenEvent(type),
		_position(position)
	{

	}

	bool TouchEvent::matches(Element *element) {
		return
			ScreenEvent::matches(element)
			&& (
				element->isCaptured()
				|| element->getBounds().intersects(_position)
			);
	}

	const Point &TouchEvent::getPosition() const {
		return _position;
	}

	void TouchEvent::setPosition(const Point &position) {
		_position = position;
	}

	TouchDownEvent::TouchDownEvent(const Point &position) : TouchEvent(
		EventType::touchDown,
		position
	) {

	}

	TouchDragEvent::TouchDragEvent(const Point &position) : TouchEvent(
		EventType::touchDrag,
		position
	) {

	}

	TouchUpEvent::TouchUpEvent(const Point &position) : TouchEvent(
		EventType::touchUp,
		position
	) {

	}

	PinchEvent::PinchEvent(const EventType &type, const Point &position1, const Point &position2) :
		ScreenEvent(type),
		_position1(position1),
		_position2(position2)
	{

	}

	const Point &PinchEvent::getPosition1() const {
		return _position1;
	}

	void PinchEvent::setPosition1(const Point &position1) {
		_position1 = position1;
	}

	const Point &PinchEvent::getPosition2() const {
		return _position2;
	}

	void PinchEvent::setPosition2(const Point &position2) {
		_position2 = position2;
	}

	bool PinchEvent::matches(Element *element) {
		return
			ScreenEvent::matches(element)
			&& (
				element->isCaptured()
				||
				element->getBounds().intersects(_position1)
				&& element->getBounds().intersects(_position2)
			);
	}

	PinchDownEvent::PinchDownEvent(const Point &position1, const Point &position2) : PinchEvent(
		EventType::pinchDown,
		position1,
		position2
	) {

	}

	PinchDragEvent::PinchDragEvent(const Point &position1, const Point &position2) : PinchEvent(
		EventType::pinchDrag,
		position1,
		position2
	) {

	}

	PinchUpEvent::PinchUpEvent(const Point &position1, const Point &position2) : PinchEvent(
		EventType::pinchUp,
		position1,
		position2
	) {

	}

	ScreenEvent::ScreenEvent(EventType type) : Event(type) {

	}
}