#pragma once

#include <cstdint>
#include <string_view>

#include <YOBA/Core/Events/Event.hpp>
#include <YOBA/Core/Key.hpp>

namespace YOBA {
	class KeyEvent : public Event {
		public:
			KeyEvent(uint16_t typeID, Key key, std::string_view text);

			Key getKey() const;
			std::string_view getText() const;

		private:
			Key _key;
			std::string_view _text;
	};

	class KeyDownEvent : public KeyEvent {
		public:
			explicit KeyDownEvent(Key key, std::string_view text);

			static uint16_t typeID;
	};

	class KeyUpEvent : public KeyEvent {
		public:
			explicit KeyUpEvent(Key key, std::string_view text);

			static uint16_t typeID;
	};
}