#pragma once

#include <YOBA/main/events/event.h>

namespace YOBA {
	class ScreenEvent : public Event {
		public:
			explicit ScreenEvent(uint16_t typeID);

			static bool isScreen(const Event* event);
	};
}