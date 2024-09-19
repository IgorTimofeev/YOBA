#pragma once

#include "Arduino.h"
#include "cstdlib"
#include "size.h"
#include "glyph.h"

namespace yoba {
	class Font {
		public:
			Font(
				uint16_t fromCodepoint,
				uint16_t toCodepoint,
				uint8_t height,
				const Glyph* glyphs,
				const uint8_t* bitmap
			);

			static const uint8_t missingGlyphWidth = 10;

			uint16_t getFromCodepoint() const;
			uint16_t getToCodepoint() const;
			const Glyph* getGlyphs() const;
			const uint8_t* getBitmap() const;
			const Glyph* getGlyph(uint16_t codepoint) const;
			uint16_t getWidth(const wchar_t *text) const;
			uint8_t getHeight() const;
			Size getSize(const wchar_t *text) const;

		private:

			const uint16_t _fromCodepoint;
			const uint16_t _toCodepoint;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}