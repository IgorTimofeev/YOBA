#pragma once

#include <functional>
#include <string>

#include <YOBA/Core/Font.hpp>

namespace YOBA {
	// I love C++
	namespace TextParsingOptions {
		enum : uint8_t {
			none =         0b00000000,
			number =       0b00000001,
			sign =         0b00000010,
			decimalPoint = 0b00000100
		};
	}

	class Text {
		public:
			static void wrap(
				const Font* font,
				const std::string_view text,
				const uint8_t scale,
				const uint16_t availableWidth,
				const std::function<void(std::string_view, uint16_t width)>& lineHandler
			);

			static bool containsIgnoreCase(std::string_view haystack, std::string_view needle);

			static bool tryParseInt32(const std::string_view text, int32_t& result);
			static bool tryParseFloat(const std::string_view text, float& result);
			static bool tryParseHex(const std::string_view text, int32_t& result);

			static int32_t tryParseInt32Or(const std::string_view text, const int32_t fallbackValue = 0);
			static float tryParseFloatOr(const std::string_view text, const float fallbackValue = 0);
			static int32_t tryParseHexOr(const std::string_view text, const int32_t fallbackValue = 0);
	};
}
