#pragma once

#include "Arduino.h"
#include "cstdlib"
#include "size.h"
#include "glyph.h"

namespace yoba {
	class Font {
		public:
			Font(
				uint32_t fromCodepoint,
				uint32_t toCodepoint,
				uint8_t height,
				const Glyph* glyphs,
				const uint8_t* bitmap
			);

			static const uint8_t missingGlyphWidth = 10;

			uint32_t getFromCodepoint() const;
			uint32_t getToCodepoint() const;
			const Glyph* getGlyphs() const;
			const uint8_t* getBitmap() const;

			template<typename TChar>
			const Glyph* getGlyph(TChar codepoint) const;

			template<typename TChar>
			uint16_t getWidth(const TChar *text) const;

			uint8_t getHeight() const;
			Size getSize(const wchar_t *text) const;

		private:
			const uint32_t _fromCodepoint;
			const uint32_t _toCodepoint;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};

	template<typename TChar>
	const Glyph *Font::getGlyph(TChar codepoint) const {
		return
			codepoint < _fromCodepoint || codepoint > _toCodepoint
			? nullptr :
			&_glyphs[codepoint - _fromCodepoint];
	}

	template<typename TChar>
	uint16_t Font::getWidth(const TChar *text) const {
		wchar_t ch;
		size_t charIndex = 0;
		const Glyph* glyph;

		uint16_t width = 0;

		while (true) {
			ch = text[charIndex];

			// End of text
			if (ch == '\0')
				break;

			// Trying to find glyph matched to char
			glyph = getGlyph(ch);

			// If glyph was found in bitmap
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
}