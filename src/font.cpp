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

	uint16_t Font::getWidth(const std::wstring_view& text) const {
		return getWidth<wchar_t>(text);
	}

	uint8_t Font::getHeight() const {
		return _height;
	}
}