#pragma once

#include <cstdint>

#include <YOBA/main/events/event.h>
#include <YOBA/UI/element.h>

namespace YOBA {
	class ScrollIntoViewEvent : public Event {
		public:
			explicit ScrollIntoViewEvent(const Element* element);

			static uint16_t typeID;

			const Element* getElement() const;

		private:
			const Element* _element;
	};
}