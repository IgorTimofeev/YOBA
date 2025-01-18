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

			const Glyph* getGlyph(wchar_t codepoint) const;
			uint8_t getCharWidth(wchar_t codepoint, uint8_t scale = 1) const;

			uint16_t getWidth(const std::wstring_view& text, uint8_t scale = 1) const;

			uint8_t getHeight() const;
			uint8_t getHeight(uint8_t scale) const;

		private:
			const uint32_t _fromCodepoint;
			const uint32_t _toCodepoint;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}