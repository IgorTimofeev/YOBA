#pragma once

#include <cstdint>

#include <YOBA/main/events/screenEvent.h>
#include <YOBA/main/vector2.h>

namespace YOBA {
	class PointerEvent : public ScreenEvent {
		public:
			PointerEvent(uint16_t typeID, const Point& position);

			const Point& getPosition() const;
			void setPosition(const Point& position);

			static bool isPointer(const Event& event);

		private:
			Point _position;
	};

	class PointerDownEvent : public PointerEvent {
		public:
			explicit PointerDownEvent(const Point& position);

			static uint16_t typeID;
	};

	class PointerDragEvent : public PointerEvent {
		public:
			explicit PointerDragEvent(const Point& position);

			static uint16_t typeID;
	};

	class PointerUpEvent : public PointerEvent {
		public:
			explicit PointerUpEvent(const Point& position);

			static uint16_t typeID;
	};
}