#include <YOBA/core/utf-8.h>

namespace YOBA {
	uint32_t UTF8::nextCodepoint(const std::string_view str, size_t& index) {
		if (str.empty() || index >= str.length())
			return 0;

		const auto byte1 = static_cast<uint8_t>(str[index]);

		// 0xxxxxxx -> 1 char string (ASCII)
		if ((byte1 & 0b1000'0000) == 0) {
			index += 1;

			return byte1;
		}

		// 110xxxxx -> 2 chars string
		if ((byte1 & 0b1110'0000) == 0b1100'0000) {
			if (index + 1 >= str.length()) {
				index = str.length();
				return 0;
			}

			const uint8_t byte2 = static_cast<uint8_t>(str[index + 1]);

			index += 2;

			return ((byte1 & 0b0001'1111) << 6) | (byte2 & 0b0011'1111);
		}

		// 1110xxxx -> 3 chars string
		if ((byte1 & 0b1111'0000) == 0b1110'0000) {
			if (index + 2 >= str.length()) {
				index = str.length();
				return 0;
			}

			const auto byte2 = static_cast<uint8_t>(str[index + 1]);
			const auto byte3 = static_cast<uint8_t>(str[index + 2]);

			index += 3;

			return ((byte1 & 0b0000'1111) << 12) | ((byte2 & 0b0011'1111) << 6) | (byte3 & 0b0011'1111);
		}

		// 1110xxxx -> 4 chars string
		if ((byte1 & 0b1111'1000) == 0b1111'0000) {
			if (index + 3 >= str.length()) {
				index = str.length();
				return 0;
			}

			const auto byte2 = static_cast<uint8_t>(str[index + 1]);
			const auto byte3 = static_cast<uint8_t>(str[index + 2]);
			const auto byte4 = static_cast<uint8_t>(str[index + 3]);

			index += 4;

			return ((byte1 & 0b0000'0111) << 18) | ((byte2 & 0b0011'1111) << 12) | ((byte3 & 0b0011'1111) << 6) | (byte4 & 0b0011'1111);
		}

		// Some weird shit like corrupted char or if we've got into middle of UTF sequence
		index += 1;

		// Falling back to replacement char �
		return 0xFFFD;
	}

	uint32_t UTF8::getLength(const std::string_view str) {
		size_t length = 0;

		for (const auto ch : str) {
			// If byte doesn't start with 10xxxxxx, i.e. it's not a continuation byte - then it fr start of a new char
			if ((static_cast<uint8_t>(ch) & 0b1100'0000) != 0b1000'0000) {
				length++;
			}
		}

		return length;
	}
}