#include <YOBA/Core/Text.hpp>
#include <YOBA/Core/UTF-8.hpp>

namespace YOBA {
	void Text::wrap(
		const Font* font,
		const std::string_view text,
		const uint8_t scale,
		const uint16_t availableWidth,
		const std::function<void(std::string_view, uint16_t width)>& lineHandler
	) {
		size_t charIndex = 0;
		size_t nextCharIndex = 0;

		uint32_t codepoint;
		uint8_t codepointWidth;

		size_t lineCharIndex = 0;
		uint16_t lineWidth = 0;

		size_t breakingCharIndex = 0;
		uint16_t breakingCharWidth = 0;
		uint16_t breakingCharLineWidth = 0;
		bool breakingCharSkip = false;

		while (nextCharIndex < text.length()) {
			UTF8::nextCodepoint(text, nextCharIndex, codepoint);

			switch (codepoint) {
				// Retarded char, should skip
				case '\r': continue;
				// Line ending, should wrap
				case '\n': {
					if (lineWidth > 0) {
						lineHandler(std::string_view(text.data() + lineCharIndex, charIndex - lineCharIndex), lineWidth);
					}
					else {
						lineHandler(std::string_view(text.data(), 0), 0);
					}

					lineCharIndex = charIndex + 1;
					lineWidth = 0;

					breakingCharIndex = 0;
					breakingCharLineWidth = 0;

					break;
				}
				// Any char
				default: {
					codepointWidth = font->getWidth(scale, codepoint);

					// Line doesn't fit, should wrap
					if (lineWidth + codepointWidth > availableWidth) {
						switch (codepoint) {
							// Whitespace
							case ' ':
							case '\t': {
								if (lineWidth > 0)
									lineHandler(std::string_view(text.data() + lineCharIndex, charIndex - lineCharIndex), lineWidth);

								lineCharIndex = charIndex + 1;
								lineWidth = 0;

								breakingCharIndex = 0;
								breakingCharLineWidth = 0;

								break;
							}
							// Any char
							default: {
								// There was breaking char somewhere
								if (breakingCharIndex > lineCharIndex) {
									if (breakingCharLineWidth > 0)
										lineHandler(std::string_view(text.data() + lineCharIndex, breakingCharIndex - lineCharIndex + (breakingCharSkip ? 0 : 1)), breakingCharLineWidth);

									lineCharIndex = breakingCharIndex + 1;
									lineWidth = lineWidth - breakingCharLineWidth - breakingCharWidth + codepointWidth;

									breakingCharIndex = 0;
									breakingCharLineWidth = 0;
								}
								// Ugly case, cutting line into 2 parts
								else {
									if (lineWidth > 0)
										lineHandler(std::string_view(text.data() + lineCharIndex, charIndex - lineCharIndex), lineWidth);

									lineCharIndex = charIndex;
									lineWidth = 0;

									breakingCharIndex = 0;
									breakingCharLineWidth = 0;
								}

								break;
							}
						}
					}
					// Line fits
					else {
						switch (codepoint) {
							// Breaking char
							case ' ':
							case '\t':
							case '-': {
								breakingCharSkip = codepoint != '-';

								// Line contains something
								if (lineWidth > 0) {
									breakingCharIndex = charIndex;
									breakingCharLineWidth = lineWidth;
									breakingCharWidth = codepointWidth;

									lineWidth += codepointWidth;
								}
								// Start of line, no need to start from breaking char
								else if (breakingCharSkip) {
									lineCharIndex = charIndex + 1;
									lineWidth = 0;

									breakingCharIndex = 0;
									breakingCharLineWidth = 0;
								}

								break;
							}
							// Any char
							default: {
								lineWidth += codepointWidth;

								break;
							}
						}
					}

					break;
				}
			}

			charIndex = nextCharIndex;
		}

		// Remaining non-wrapped part
		if (lineWidth > 0) {
			lineHandler(std::string_view(text.data() + lineCharIndex, text.length() - lineCharIndex), lineWidth);
		}
	}

	bool Text::containsIgnoreCase(std::string_view haystack, std::string_view needle) {
		return !std::ranges::search(
			haystack,
			needle,
			[](const char a, const char b) {
				return std::tolower(a) == std::tolower(b);
			}
		).empty();
	}

	bool Text::tryParseInt32(const std::string_view text, int32_t& result) {
		char* endPtr;
		result = std::strtol(text.data(), &endPtr, 10);

		return endPtr != text.data();
	}

	bool Text::tryParseFloat(const std::string_view text, float& result) {
		char* endPtr;
		result = std::strtof(text.data(), &endPtr);

		return endPtr != text.data();
	}

	bool Text::tryParseHex(const std::string_view text, int32_t& result) {
		char* endPtr;
		result = std::strtol(text.data(), &endPtr, 16);

		return endPtr != text.data();
	}

	int32_t Text::tryParseInt32Or(const std::string_view text, const int32_t fallbackValue) {
		int32_t result = 0;

		if (!tryParseInt32(text, result))
			result = fallbackValue;

		return result;
	}

	float Text::tryParseFloatOr(const std::string_view text, const float fallbackValue) {
		float result = 0;

		if (!tryParseFloat(text, result))
			result = fallbackValue;

		return result;
	}

	int32_t Text::tryParseHexOr(const std::string_view text, const int32_t fallbackValue) {
		int32_t result = 0;

		if (!tryParseHex(text, result))
			result = fallbackValue;

		return result;
	}
}
