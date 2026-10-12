#pragma once

#include <cstdint>

namespace YOBA {
	enum class KeyboardKeyType : uint8_t {
		normal,
		charactersLayout,
		cyclicLayout,
		action
	};

	enum class KeyboardCase : uint8_t {
		lower,
		upper,
		caps
	};
}