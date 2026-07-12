#include <YOBA/Core/Font.hpp>
#include <YOBA/Core/UTF-8.hpp>

namespace YOBA {
	uint16_t Font::getWidth(const std::string_view text) const {
		uint16_t width = 0;
		uint32_t codepoint;
		size_t charIndex = 0;

		while (charIndex < text.length()) {
			UTF8::nextCodepoint(text, charIndex, codepoint);
			width += getWidth(codepoint);
		}

		return width;
	}

	uint16_t Font::getWidth(const std::string_view text, const uint8_t scale) const {
		return getWidth(text) * scale;
	}

	Size Font::getSize(const std::string_view text) const {
		return {
			getWidth(text),
			getLineHeight()
		};
	}

	Size Font::getSize(const std::string_view text, const uint8_t scale) const {
		return {
			getWidth(text, scale),
			getLineHeight(scale)
		};
	}
}