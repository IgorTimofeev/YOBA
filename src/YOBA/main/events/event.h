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

			template<class T>
			requires std::derived_from<T, Event>
			T* castTo() {
				return
					T::typeID == this->_typeID
					? static_cast<T*>(this)
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