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
					codepointWidth = font->getWidth(codepoint, scale);

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
}