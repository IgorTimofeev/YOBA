#pragma once

#include <cstdint>
#include <string_view>
#include <optional>

#include <YOBA/main/events/event.h>
#include <YOBA/main/key.h>

namespace YOBA {
	class KeyEvent : public Event {
		public:
			explicit KeyEvent(uint16_t& staticTypeID, Key key, const std::optional<std::wstring_view>& text);

			Key getKey() const;
			const std::optional<std::wstring_view>& getText() const;

		private:
			Key _key;
			std::optional<std::wstring_view> _text;
	};

	class KeyDownEvent : public KeyEvent {
		public:
			explicit KeyDownEvent(Key key, const std::optional<std::wstring_view>& text);

			static uint16_t typeID;
	};

	class KeyUpEvent : public KeyEvent {
		public:
			explicit KeyUpEvent(Key key, const std::optional<std::wstring_view>& text);

			static uint16_t typeID;
	};
}