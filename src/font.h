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
			uint8_t getCharWidth(TChar codepoint) const;

			template<typename TChar>
			uint16_t getWidth(const std::basic_string_view<TChar>& text) const;

			uint8_t getHeight() const;
			Size getSize(const std::wstring_view& text) const;

		private:
			const uint32_t _fromCodepoint;
			const uint32_t _toCodepoint;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};

	template<typename TChar>
	const Glyph* Font::getGlyph(TChar codepoint) const {
		return
			codepoint < _fromCodepoint || codepoint > _toCodepoint
			? nullptr
			: &_glyphs[codepoint - _fromCodepoint];
	}

	template<typename TChar>
	uint8_t Font::getCharWidth(TChar codepoint) const {
		const auto glyph = getGlyph(codepoint);

		return glyph ? glyph->getWidth() : missingGlyphWidth;
	}

	template<typename TChar>
	uint16_t Font::getWidth(const std::basic_string_view<TChar>& text) const {
		uint16_t width = 0;

		for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
			width += getCharWidth(text[charIndex]);

		return width;
	}
}