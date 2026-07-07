#pragma once

#include <cstdint>
#include <YOBA/Core/Events/Event.hpp>

namespace YOBA {
	class ScreenEvent : public Event {
		public:
			explicit ScreenEvent(uint16_t typeID);

			static bool isScreen(const Event* event);
	};
}