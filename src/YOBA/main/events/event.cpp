#include "event.h"

namespace YOBA {
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
}