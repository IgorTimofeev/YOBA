#pragma once

#include <cstdlib>
#include <string>
#include <functional>

#include <YOBA/main/glyph.h>
#include <YOBA/main/size.h>

namespace YOBA {
	class Font {
		public:
			constexpr Font(
				const uint32_t fromCodepoint,
				const uint32_t toCodepoint,
				const uint8_t glyphWidth,
				const uint8_t glyphHeight,
				const Glyph *glyphs,
				const uint8_t *bitmap
			) :
				fromCodepoint(fromCodepoint),
				toCodepoint(toCodepoint),
				glyphWidth(glyphWidth),
				glyphHeight(glyphHeight),
				glyphs(glyphs),
				bitmap(bitmap)
			{
			
			}

			constexpr static uint8_t monospacedGlyphWidth = 0;
			constexpr static uint8_t missingGlyphWidth = 10;

			constexpr bool isMonospaced() const {
				return glyphWidth == monospacedGlyphWidth;
			}

			const uint8_t* getBitmap() const;
			const Glyph* getGlyph(uint32_t codepoint) const;

			uint8_t getWidth(uint32_t codepoint) const;
			uint8_t getWidth(uint32_t codepoint, uint8_t scale) const;

			uint8_t getWidth(const Glyph* glyph) const;
			uint8_t getWidth(const Glyph* glyph, uint8_t scale) const;

			uint16_t getWidth(std::string_view text) const;
			uint16_t getWidth(std::string_view text, uint8_t scale) const;

			uint16_t getWidth(std::wstring_view text) const;
			uint16_t getWidth(std::wstring_view text, uint8_t scale) const;

			uint8_t getHeight() const;
			uint8_t getHeight(uint8_t scale) const;
			void wrap(std::string_view text, uint8_t scale, uint16_t maxWidth, const std::function<void(std::string_view, uint16_t width)>& lineHandler) const;
			void wrap(std::wstring_view text, uint8_t scale, uint16_t maxWidth, const std::function<void(std::wstring_view, uint16_t width)>& lineHandler) const;

		private:
			const uint32_t fromCodepoint;
			const uint32_t toCodepoint;
			const uint8_t glyphWidth;
			const uint8_t glyphHeight;
			const Glyph* glyphs;
			const uint8_t* bitmap;
	};
}