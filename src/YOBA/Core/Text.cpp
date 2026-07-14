#include <YOBA/Core/Text.hpp>
#include <YOBA/Core/UTF-8.hpp>

namespace YOBA {
	void Text::wrap(
		const Font* font,
		const std::string_view text,
		const uint8_t scale,
		const uint16_t maxWidth,
		const std::function<void(std::string_view, uint16_t width)>& lineHandler
	) {
		size_t charIndex = 0;
		uint8_t charWidth;

		uint32_t codepoint;
		size_t codepointIndex = 0;

		size_t lineCharIndex = 0;
		uint16_t lineWidth = 0;

		size_t breakingCharIndex = 0;
		uint16_t breakingCharWidth = 0;
		uint16_t breakingCharLineWidth = 0;
		bool breakingCharSkip = false;

		while (charIndex < text.length()) {
			UTF8::nextCodepoint(text, charIndex, codepoint);

			switch (codepoint) {
				// Retarded char, should skip
				case '\r': continue;
				// Line ending, should wrap
				case '\n': {
					if (lineWidth > 0) {
						lineHandler(UTF8::substring(text, lineCharIndex, codepointIndex - lineCharIndex), lineWidth);
					}
					else {
						lineHandler(std::string(), 0);
					}

					lineCharIndex = codepointIndex + 1;
					lineWidth = 0;

					breakingCharIndex = 0;
					breakingCharLineWidth = 0;

					break;
				}
				// Any char
				default: {
					charWidth = font->getWidth(codepoint, scale);

					// Line doesn't fit, should wrap
					if (lineWidth + charWidth > maxWidth) {
						switch (codepoint) {
							// Whitespace
							case ' ': {
								if (lineWidth > 0)
									lineHandler(UTF8::substring(text, lineCharIndex, codepointIndex - lineCharIndex), lineWidth);

								lineCharIndex = codepointIndex + 1;
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
										lineHandler(UTF8::substring(text, lineCharIndex, breakingCharIndex - lineCharIndex + (breakingCharSkip ? 0 : 1)), breakingCharLineWidth);

									lineCharIndex = breakingCharIndex + 1;
									lineWidth = lineWidth - breakingCharLineWidth - breakingCharWidth + charWidth;

									breakingCharIndex = 0;
									breakingCharLineWidth = 0;
								}
								// Ugly case, cutting line into 2 parts
								else {
									if (lineWidth > 0)
										lineHandler(UTF8::substring(text, lineCharIndex, codepointIndex - lineCharIndex), lineWidth);

									lineCharIndex = codepointIndex;
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
							case '-': {
								breakingCharSkip = codepoint == ' ';

								// Line contains something
								if (lineWidth > 0) {
									breakingCharIndex = codepointIndex;
									breakingCharLineWidth = lineWidth;
									breakingCharWidth = charWidth;

									lineWidth += charWidth;
								}
								// Start of line, no need to start from breaking char
								else if (breakingCharSkip) {
									lineCharIndex = codepointIndex + 1;
									lineWidth = 0;

									breakingCharIndex = 0;
									breakingCharLineWidth = 0;
								}

								break;
							}
							// Any char
							default: {
								lineWidth += charWidth;

								break;
							}
						}
					}

					break;
				}
			}

			codepointIndex++;
		}

		// Remaining non-wrapped part
		if (lineWidth > 0) {
			lineHandler(UTF8::substring(text, lineCharIndex), lineWidth);
		}
	}
}