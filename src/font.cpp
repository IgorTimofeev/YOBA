#include "font.h"

namespace yoba {
	Font::Font(
		uint32_t fromCodepoint,
		uint32_t toCodepoint,
		uint8_t height,
		const Glyph *glyphs,
		const uint8_t *bitmap
	) :
		_fromCodepoint(fromCodepoint),
		_toCodepoint(toCodepoint),
		_height(height),
		_glyphs(glyphs),
		_bitmap(bitmap)
	{

	}

	uint32_t Font::getFromCodepoint() const {
		return _fromCodepoint;
	}

	uint32_t Font::getToCodepoint() const {
		return _toCodepoint;
	}

	const Glyph *Font::getGlyphs() const {
		return _glyphs;
	}

	const uint8_t *Font::getBitmap() const {
		return _bitmap;
	}

	const Glyph* Font::getGlyph(wchar_t codepoint) const {
		return
			codepoint < _fromCodepoint || codepoint > _toCodepoint
			? nullptr
			: &_glyphs[codepoint - _fromCodepoint];
	}

	uint8_t Font::getCharWidth(wchar_t codepoint, uint8_t scale) const {
		const auto glyph = getGlyph(codepoint);

		return (glyph ? glyph->getWidth() : missingGlyphWidth) * scale;
	}

	uint16_t Font::getWidth(const std::wstring_view& text, uint8_t scale) const {
		uint16_t width = 0;

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
			width += getCharWidth(text[charIndex], scale);

		return width;
	}

	uint8_t Font::getHeight() const {
		return _height;
	}

	uint8_t Font::getHeight(uint8_t scale) const {
		return _height * scale;
	}
}