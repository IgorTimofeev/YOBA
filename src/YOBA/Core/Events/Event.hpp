#pragma once

#include <concepts>
#include <cstdint>

namespace YOBA {
	class Event {
		public:
			explicit Event(uint16_t typeID);
			virtual ~Event() = default;

			bool isHandled() const;
			void setHandled(bool handled);
			uint16_t getTypeID() const;

			/**
			* @brief Convenient shorthand for event type checking. For example, instead of
			* @code
			* if (event->getTypeID() == TouchDownEvent::typeID) {
			*   ...
			* }
			* @endcode
			* you can use
			* @code
			* if (event->is<TouchDownEvent>()) {
			*   ...
			* }
			* @endcode
			*
			* @return <b>true</b> if event types matches required one, and <b>false</b> otherwise
			*/
			template<class TEvent>
			requires std::derived_from<TEvent, Event>
			bool is() const {
				return TEvent::typeID == this->_typeID;
			}

			/**
			* @brief Convenient shorthand for event type casting. For example, instead of
			* @code
			* if (event->is<TouchDownEvent>()) {
			*   const auto touchDownEvent = static_cast<TouchDownEvent*>(event);
			*   const auto position = touchDownEvent->getPosition();
			* }
			* @endcode
			* you can use
			* @code
			* if (const auto touchDownEvent = event->as<TouchDownEvent>()) {
			*   const auto position = touchDownEvent.getPosition();
			* }
			* @endcode
			*
			* @return <b>TEvent*</b> if event types matches <b>TEvent</b>, and <b>nullptr</b> otherwise
			*/
			template<class TEvent>
			requires std::derived_from<TEvent, Event>
			TEvent* as() {
				return
					this->is<TEvent>()
					? static_cast<TEvent*>(this)
					: nullptr;
			}

		protected:
			static uint16_t registerTypeID();

		private:
			static uint16_t _nextTypeID;

			uint16_t _typeID = 0;
			bool _handled = false;
	};
}