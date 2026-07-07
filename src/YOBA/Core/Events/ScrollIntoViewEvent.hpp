#pragma once

#include <cstdint>

#include <YOBA/Core/Events/Event.hpp>
#include <YOBA/UI/Element.hpp>

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