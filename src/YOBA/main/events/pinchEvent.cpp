#include "pinchEvent.h"

#include <cstdint>

namespace YOBA {
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