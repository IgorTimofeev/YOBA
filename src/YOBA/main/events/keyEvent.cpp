#include <YOBA/main/events/keyEvent.h>

namespace YOBA {
	KeyEvent::KeyEvent(const uint16_t typeID, const Key key, const std::optional<std::wstring_view>& text) : Event(typeID), _key(key), _text(text) {

	}

	Key KeyEvent::getKey() const {
		return _key;
	}

	const std::optional<std::wstring_view>& KeyEvent::getText() const {
		return _text;
	}

	// Down
	KeyDownEvent::KeyDownEvent(const Key key, const std::optional<std::wstring_view>& text) : KeyEvent(typeID, key, text) {

	}

	uint16_t KeyDownEvent::typeID = registerTypeID();

	// Up
	KeyUpEvent::KeyUpEvent(const Key key, const std::optional<std::wstring_view>& text) : KeyEvent(typeID, key, text) {

	}

	uint16_t KeyUpEvent::typeID = registerTypeID();
}