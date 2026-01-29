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

	namespace KeyboardLayoutOptions {
		enum : uint8_t {
			text = 0b0000'0001,
			numeric = 0b0000'0010,

			allowSigned = 0b0000'1000,
			allowDecimal = 0b0001'0000
		};
	}
}