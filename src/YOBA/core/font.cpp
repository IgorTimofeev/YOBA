#include "font.h"

#include "utf-8.h"

namespace YOBA {
	uint16_t Font::getWidth(const std::string_view text) const {
		uint16_t width = 0;

		size_t charIndex = 0;

		while (charIndex < text.length()) {
			width += getWidth(UTF8::nextCodepoint(text, charIndex));
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