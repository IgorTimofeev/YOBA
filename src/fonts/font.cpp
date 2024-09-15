#include "font.h"

namespace yoba {
	Font::Font(uint16_t fromCodepoint, uint16_t toCodepoint, uint8_t height, const Glyph *glyphs, const uint8_t *bitmap) :
		_fromCodepoint(fromCodepoint),
		_toCodepoint(toCodepoint),
		_height(height),
		_glyphs(glyphs),
		_bitmap(bitmap)
	{

	}

	uint16_t Font::getFromCodepoint() const {
		return _fromCodepoint;
	}

	uint16_t Font::getToCodepoint() const {
		return _toCodepoint;
	}

	const Glyph *Font::getGlyphs() const {
		return _glyphs;
	}

	const uint8_t *Font::getBitmap() const {
		return _bitmap;
	}

	const Glyph *Font::getGlyph(uint32_t codepoint) const {
		return
			codepoint < _fromCodepoint || codepoint > _toCodepoint
			? nullptr :
			&_glyphs[codepoint - _fromCodepoint];
	}

	uint16_t Font::getWidth(const char *text) const {
		const char* charPtr;
		size_t charIndex = 0;
		const Glyph* glyph;

		uint16_t width = 0;

		while (true) {
			charPtr = text + charIndex;

			// End of text
			if (*charPtr == '\0')
				break;

			// Trying to find glyph matched to char
			glyph = getGlyph(*charPtr);

			if (glyph) {
				width += glyph->getWidth();
			}
				// For non-existing glyphs we can just simulate whitespace
			else {
				width += missingGlyphWidth;
			}

			charIndex++;
		}

		return width;
	}

	uint16_t Font::getWidth(const String &text) const {
		return getWidth(text.c_str());
	}

	uint8_t Font::getHeight() const {
		return _height;
	}

	Size Font::getSize(const char *text) const {
		return {
			getWidth(text),
			getHeight()
		};
	}

	Size Font::getSize(const String &text) const {
		return getSize(text.c_str());
	}
}