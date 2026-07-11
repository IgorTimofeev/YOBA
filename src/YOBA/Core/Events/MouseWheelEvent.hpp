#pragma once

#include <cstdint>

#include <YOBA/Core/Events/PointerEvent.hpp>

namespace YOBA {
	// Desktop only
	class MouseWheelEvent : public PointerEvent {
		public:
			MouseWheelEvent(const Point& position, int32_t delta);

			int32_t getDelta() const;
			void setDelta(const int32_t delta);

			static uint16_t typeID;

		private:
			int32_t _delta;
	};
}
