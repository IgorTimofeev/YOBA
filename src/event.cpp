#include "event.h"
#include "ui/element.h"

namespace yoba {
	// -------------------------------- Event --------------------------------

	uint16_t Event::_nextTypeID = 0;

	void Event::registerTypeID(uint16_t& staticTypeID) {
		if (staticTypeID == 0) {
			staticTypeID = _nextTypeID++;
		}

		_typeID = staticTypeID;
	}

	bool Event::isHandled() const {
		return _handled;
	}

	void Event::setHandled(bool handled) {
		_handled = handled;
	}

	bool Event::matches(Element* element) {
		return true;
	}

	uint16_t Event::getTypeID() const {
		return _typeID;
	}

	// -------------------------------- Screen --------------------------------

	bool ScreenEvent::matches(Element* element) {
		return element->isVisible() && element->isEnabled();
	}

	// -------------------------------- Touch --------------------------------

	TouchEvent::TouchEvent(const Point& position) :
		_position(position)
	{

	}

	bool TouchEvent::matches(Element* element) {
		return
			ScreenEvent::matches(element)
			&& (
				element->isCaptured()
				|| element->getBounds().intersects(_position)
			);
	}

	const Point& TouchEvent::getPosition() const {
		return _position;
	}

	void TouchEvent::setPosition(const Point& position) {
		_position = position;
	}

	TouchDownEvent::TouchDownEvent(const Point& position) : TouchEvent(
		position
	) {
		registerTypeID(typeID);
	}

	TouchDragEvent::TouchDragEvent(const Point& position) : TouchEvent(
		position
	) {
		registerTypeID(typeID);
	}

	TouchUpEvent::TouchUpEvent(const Point& position) : TouchEvent(
		position
	) {
		registerTypeID(typeID);
	}

	PinchEvent::PinchEvent(const Point& position1, const Point& position2) :
		_position1(position1),
		_position2(position2)
	{

	}

	const Point& PinchEvent::getPosition1() const {
		return _position1;
	}

	void PinchEvent::setPosition1(const Point& position1) {
		_position1 = position1;
	}

	const Point& PinchEvent::getPosition2() const {
		return _position2;
	}

	void PinchEvent::setPosition2(const Point& position2) {
		_position2 = position2;
	}

	bool PinchEvent::matches(Element* element) {
		return
			ScreenEvent::matches(element)
			&& (
				element->isCaptured()
				||
				element->getBounds().intersects(_position1)
				&& element->getBounds().intersects(_position2)
			);
	}

	PinchDownEvent::PinchDownEvent(const Point& position1, const Point& position2) : PinchEvent(
		position1,
		position2
	) {
		registerTypeID(typeID);
	}

	PinchDragEvent::PinchDragEvent(const Point& position1, const Point& position2) : PinchEvent(
		position1,
		position2
	) {
		registerTypeID(typeID);
	}

	PinchUpEvent::PinchUpEvent(const Point& position1, const Point& position2) : PinchEvent(
		position1,
		position2
	) {
		registerTypeID(typeID);
	}
}