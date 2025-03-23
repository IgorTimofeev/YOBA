#pragma once

#include <string>
#include <functional>
#include "cstdlib"
#include "size.h"
#include "glyph.h"

namespace yoba {
	class Font {
		public:
			Font(
				uint32_t fromCodepoint,
				uint32_t toCodepoint,
				uint8_t width,
				uint8_t height,
				const Glyph* glyphs,
				const uint8_t* bitmap
			);

			constexpr static const uint8_t missingGlyphWidth = 10;

			uint32_t getFromCodepoint() const;
			uint32_t getToCodepoint() const;
			const Glyph* getGlyphs() const;
			const uint8_t* getBitmap() const;

			const Glyph* getGlyph(wchar_t codepoint) const;

			uint8_t getCharWidth(wchar_t codepoint) const;
			uint8_t getCharWidth(wchar_t codepoint, uint8_t scale) const;

			uint8_t getGlyphWidth(const Glyph* glyph) const;
			uint8_t getGlyphWidth(const Glyph* glyph, uint8_t scale) const;

			uint16_t getWidth(std::wstring_view text) const;
			uint16_t getWidth(std::wstring_view text, uint8_t scale) const;

			uint8_t getHeight() const;
			uint8_t getHeight(uint8_t scale) const;

			void wrap(std::wstring_view text, uint8_t scale, uint16_t maxWidth, const std::function<void(std::wstring_view, uint16_t width)>& lineHandler) const;

		private:
			const uint32_t _fromCodepoint;
			const uint32_t _toCodepoint;
			const uint8_t _width;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}