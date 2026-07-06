#include <YOBA/core/text.h>
#include <YOBA/core/utf-8.h>

namespace YOBA {
	void Text::wrap(
		const Font* font,
		const std::string_view text,
		const uint8_t scale,
		const uint16_t maxWidth,
		const std::function<void(std::string_view, uint16_t width)>& lineHandler
	) {
		size_t
			lineFrom = 0,
			spaceAt = 0;

		uint8_t charWidth;

		uint16_t
			spaceWidth = 0,
			lineWidth = 0,
			spaceLineWidth = 0;

		uint32_t codepoint;

		// 01234567
		// He pizd
		size_t charIndex = 0;

		while (charIndex < text.length()) {
			codepoint = UTF8::nextCodepoint(text, charIndex);

			switch (codepoint) {
				// Retarded char, should skip
				case '\r': continue;
				// Line ending, should wrap
				case '\n': {
					if (lineWidth > 0) {
						lineHandler(text.substr(lineFrom, charIndex - 1 - lineFrom), lineWidth);
					}
					else {
						lineHandler(std::string(), 0);
					}

					lineFrom = charIndex - 1 + 1;

					spaceAt = 0;
					spaceLineWidth = 0;
					lineWidth = 0;

					break;
				}
				// Any char
				default: {
					charWidth = font->getWidth(codepoint, scale);

					// Line doesn't fit, should wrap
					if (lineWidth + charWidth > maxWidth) {
						// Whitespace on end of line
						if (codepoint == ' ') {
							if (lineWidth > 0)
								lineHandler(text.substr(lineFrom, charIndex - 1 - lineFrom), lineWidth);

							lineFrom = charIndex - 1 + 1;

							spaceAt = 0;
							spaceLineWidth = 0;
							lineWidth = 0;
						}
						// Any char on end of  line
						else {
							// There was whitespace in the middle
							if (spaceAt > lineFrom) {
								if (spaceLineWidth > 0)
									lineHandler(text.substr(lineFrom, spaceAt - lineFrom), spaceLineWidth);

								lineFrom = spaceAt + 1;
								lineWidth = lineWidth - spaceLineWidth - spaceWidth + charWidth;

								spaceAt = 0;
								spaceLineWidth = 0;
							}
							// Ugly case, cutting line into 2 parts
							else {
								if (lineWidth > 0)
									lineHandler(text.substr(lineFrom, charIndex - 1 - lineFrom), lineWidth);

								lineFrom = charIndex - 1;

								spaceAt = 0;
								spaceLineWidth = 0;
								lineWidth = 0;
							}
						}
					}
					// Line fits
					else {
						// Whitespace
						if (codepoint == ' ') {
							// Line contains something
							if (lineWidth > 0) {
								spaceAt = charIndex - 1;
								spaceLineWidth = lineWidth;
								lineWidth += charWidth;
								spaceWidth = charWidth;
							}
							// Start of line
							else {
								lineFrom = charIndex - 1 + 1;

								spaceAt = 0;
								spaceLineWidth = 0;
								lineWidth = 0;
							}
						}
						// Any char
						else {
							lineWidth += charWidth;
						}
					}

					break;
				}
			}
		}

		// Remaining non-wrapped part
		if (lineWidth > 0) {
			lineHandler(text.substr(lineFrom), lineWidth);
		}
	}
}