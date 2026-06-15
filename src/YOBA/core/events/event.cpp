#include "event.h"

namespace YOBA {
	Event::Event(const uint16_t typeID) {
		_typeID = typeID;
	}

	uint16_t Event::_nextTypeID = 0;

	uint16_t Event::registerTypeID() {
		const auto result = _nextTypeID;

		_nextTypeID++;

		return result;
	}

	bool Event::isHandled() const {
		return _handled;
	}

	void Event::setHandled(const bool handled) {
		_handled = handled;
	}

	uint16_t Event::getTypeID() const {
		return _typeID;
	}
}
