#include "font.h"

namespace YOBA {
	Font::Font(
		uint32_t fromCodepoint,
		uint32_t toCodepoint,
		uint8_t glyphWidth,
		uint8_t glyphHeight,
		const Glyph *glyphs,
		const uint8_t *bitmap
	) :
		_fromCodepoint(fromCodepoint),
		_toCodepoint(toCodepoint),
		_glyphWidth(glyphWidth),
		_glyphHeight(glyphHeight),
		_glyphs(glyphs),
		_bitmap(bitmap)
	{

	}

	bool Font::isVariableWidth() const {
		return _glyphWidth == 0;
	}

	const uint8_t* Font::getBitmap() const {
		return _bitmap;
	}

	const Glyph* Font::getGlyph(wchar_t codepoint) const {
		return
			codepoint < _fromCodepoint || codepoint > _toCodepoint
			? nullptr
			: (
				isVariableWidth()
				? reinterpret_cast<const VariableWidthGlyph*>(_glyphs) + codepoint - _fromCodepoint
				: _glyphs + codepoint - _fromCodepoint
			);
	}

	uint8_t Font::getWidth(const Glyph* glyph) const {
		return
			glyph
			? (
				isVariableWidth()
				? reinterpret_cast<const VariableWidthGlyph*>(glyph)->getWidth()
				: _glyphWidth
			)
			: missingGlyphWidth;
	}

	uint8_t Font::getWidth(const Glyph* glyph, uint8_t scale) const {
		return getWidth(glyph) * scale;
	}

	uint8_t Font::getWidth(wchar_t codepoint) const {
		return getWidth(getGlyph(codepoint));
	}

	uint8_t Font::getWidth(wchar_t codepoint, uint8_t scale) const {
		return getWidth(codepoint) * scale;
	}

	uint16_t Font::getWidth(std::wstring_view text) const {
		uint16_t width = 0;

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
			width += getWidth(text[charIndex]);

		return width;
	}

	uint16_t Font::getWidth(std::wstring_view text, uint8_t scale) const {
		return getWidth(text) * scale;
	}

	uint8_t Font::getHeight() const {
		return _glyphHeight;
	}

	uint8_t Font::getHeight(uint8_t scale) const {
		return _glyphHeight * scale;
	}

	void Font::wrap(std::wstring_view text, uint8_t scale, uint16_t maxWidth, const std::function<void(std::wstring_view, uint16_t width)>& lineHandler) const {
		size_t
			lineFrom = 0,
			spaceAt = 0;

		uint16_t
			charWidth,
			spaceWidth = 0,
			lineWidth = 0,
			spaceLineWidth = 0;

		wchar_t ch;

		// 01234567
		// He pizd
		for (size_t i = 0; i < text.length(); i++) {
			ch = text[i];

			switch (ch) {
				// Retarded char, should skip
				case L'\r': continue;
				// Line ending, should wrap
				case L'\n': {
					if (lineWidth > 0) {
						lineHandler(text.substr(lineFrom, i - lineFrom), lineWidth);
					}
					else {
						lineHandler(std::wstring(), 0);
					}

					lineFrom = i + 1;

					spaceAt = 0;
					spaceLineWidth = 0;
					lineWidth = 0;

					break;
				}
				// Any char
				default: {
					charWidth = getWidth(ch, scale);

					// Line doesn't fit, should wrap
					if (lineWidth + charWidth > maxWidth) {
						// Whitespace on end of line
						if (ch == L' ') {
							if (lineWidth > 0)
								lineHandler(text.substr(lineFrom, i - lineFrom), lineWidth);

							lineFrom = i + 1;

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
									lineHandler(text.substr(lineFrom, i - lineFrom), lineWidth);

								lineFrom = i;

								spaceAt = 0;
								spaceLineWidth = 0;
								lineWidth = 0;
							}
						}
					}
					// Line fits
					else {
						// Whitespace
						if (ch == L' ') {
							// Line contains something
							if (lineWidth > 0) {
								spaceAt = i;
								spaceLineWidth = lineWidth;
								lineWidth += charWidth;
								spaceWidth = charWidth;
							}
							// Start of line
							else {
								lineFrom = i + 1;

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