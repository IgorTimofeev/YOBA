#include <YOBA/main/events/scrollIntoViewEvent.h>

namespace YOBA {
	ScrollIntoViewEvent::ScrollIntoViewEvent(const Element* element) : Event(typeID), _element(element) {

	}

	uint16_t ScrollIntoViewEvent::typeID = registerTypeID();

	const Element* ScrollIntoViewEvent::getElement() const {
		return _element;
	}
}
