#include <cstdint>
#include "event.h"

#include <esp_log.h>
namespace YOBA {
	// -------------------------------- Event --------------------------------

	Event::Event(uint16_t& staticTypeID) {
		if (staticTypeID == 0)
			staticTypeID = _nextTypeID++;

		_typeID = staticTypeID;
	}

	uint16_t Event::_nextTypeID = 1;

	bool Event::isHandled() const {
		return _handled;
	}

	void Event::setHandled(bool handled) {
		_handled = handled;
	}

	uint16_t Event::getTypeID() const {
		return _typeID;
	}

	// -------------------------------- Input --------------------------------

	InputEvent::InputEvent(uint16_t& staticTypeID) : Event(staticTypeID) {

	}

	// -------------------------------- Screen --------------------------------

	ScreenEvent::ScreenEvent(uint16_t& staticTypeID) : InputEvent(staticTypeID) {

	}

	bool ScreenEvent::isScreen(const Event* event) {
		return
			event->getTypeID() == TouchDownEvent::typeID
			|| event->getTypeID() == TouchDragEvent::typeID
			|| event->getTypeID() == TouchUpEvent::typeID

			|| event->getTypeID() == PinchDownEvent::typeID
			|| event->getTypeID() == PinchDragEvent::typeID
			|| event->getTypeID() == PinchUpEvent::typeID;
	}

	// -------------------------------- Touch --------------------------------

	TouchEvent::TouchEvent(uint16_t& staticTypeID, const Point& position) :
		ScreenEvent(staticTypeID),
		_position(position)
	{

	}

	const Point& TouchEvent::getPosition() const {
		return _position;
	}

	void TouchEvent::setPosition(const Point& position) {
		_position = position;
	}

	bool TouchEvent::isTouch(const Event* event) {
		return
			event->getTypeID() == TouchDownEvent::typeID
			|| event->getTypeID() == TouchDragEvent::typeID
			|| event->getTypeID() == TouchUpEvent::typeID;
	}

	TouchDownEvent::TouchDownEvent(const Point& position) : TouchEvent(
		typeID,
		position
	) {

	}

	uint16_t TouchDownEvent::typeID = 0;

	TouchDragEvent::TouchDragEvent(const Point& position) : TouchEvent(
		typeID,
		position
	) {

	}

	uint16_t TouchDragEvent::typeID = 0;

	TouchUpEvent::TouchUpEvent(const Point& position) : TouchEvent(
		typeID,
		position
	) {

	}

	uint16_t TouchUpEvent::typeID = 0;

	PinchEvent::PinchEvent(uint16_t& staticTypeID, const Point& position1, const Point& position2) :
		ScreenEvent(staticTypeID),
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

	float PinchEvent::getLength() const {
		return (_position2 - _position1).getLength();
	}

	bool PinchEvent::isPinch(const Event* event) {
		return
			event->getTypeID() == PinchDownEvent::typeID
			|| event->getTypeID() == PinchDragEvent::typeID
			|| event->getTypeID() == PinchUpEvent::typeID;
	}

	void PinchEvent::setPosition2(const Point& position2) {
		_position2 = position2;
	}

	PinchDownEvent::PinchDownEvent(const Point& position1, const Point& position2) : PinchEvent(
		typeID,
		position1,
		position2
	) {

	}

	uint16_t PinchDownEvent::typeID = 0;

	PinchDragEvent::PinchDragEvent(const Point& position1, const Point& position2) :
		PinchEvent(
			typeID,
			position1,
			position2
		)
	{

	}

	uint16_t PinchDragEvent::typeID = 0;

	PinchUpEvent::PinchUpEvent(const Point& position1, const Point& position2) : PinchEvent(
		typeID,
		position1,
		position2
	) {

	}

	uint16_t PinchUpEvent::typeID = 0;
}