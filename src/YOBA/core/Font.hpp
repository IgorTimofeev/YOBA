#pragma once

#include <cstdlib>
#include <string>

#include <YOBA/Core/Glyph.hpp>
#include <YOBA/Core/Size.hpp>

namespace YOBA {
	class Font {
		public:
			constexpr Font(
				const uint32_t fromCodepoint,
				const uint32_t toCodepoint,
				const uint8_t constantGlyphWidth,
				const uint8_t lineHeight,
				const Glyph* glyphs,
				const uint8_t* bitmap
			) :
				_fromCodepoint(fromCodepoint),
				_toCodepoint(toCodepoint),
				_constantGlyphWidth(constantGlyphWidth),
				_lineHeight(lineHeight),
				_glyphs(glyphs),
				_bitmap(bitmap)
			{

			}

			constexpr static uint8_t missingGlyphWidth = 10;

			constexpr bool isConstantGlyphWidth() const {
				return _constantGlyphWidth > 0;
			}

			constexpr uint8_t getConstantGlyphWidth() const {
				return _constantGlyphWidth;
			}

			constexpr uint32_t getFromCodepoint() const {
				return _fromCodepoint;
			}

			constexpr uint32_t getToCodepoint() const {
				return _toCodepoint;
			}

			constexpr const uint8_t* getBitmap() const {
				return _bitmap;
			}

			constexpr const Glyph* getGlyph(const uint32_t codepoint) const {
				return
					codepoint < _fromCodepoint || codepoint > _toCodepoint
					? nullptr
					: (
						isConstantGlyphWidth()
						? _glyphs + codepoint - _fromCodepoint
						: reinterpret_cast<const VariableWidthGlyph*>(_glyphs) + codepoint - _fromCodepoint
					);
			}
			constexpr uint8_t getWidth(const Glyph* glyph) const {
				return
					glyph
					? (
						isConstantGlyphWidth()
						? _constantGlyphWidth
						: reinterpret_cast<const VariableWidthGlyph*>(glyph)->getWidth()
					)
					: missingGlyphWidth;
			}

			constexpr uint8_t getWidth(const Glyph* glyph, const uint8_t scale) const {
				return getWidth(glyph) * scale;
			}

			constexpr uint8_t getWidth(const uint32_t codepoint) const {
				return getWidth(getGlyph(codepoint));
			}

			constexpr uint8_t getWidth(const uint32_t codepoint, const uint8_t scale) const {
				return getWidth(codepoint) * scale;
			}

			uint16_t getWidth(std::string_view text) const;
			uint16_t getWidth(std::string_view text, uint8_t scale) const;

			constexpr uint8_t getLineHeight() const {
				return _lineHeight;
			}

			constexpr uint8_t getLineHeight(const uint8_t scale) const {
				return _lineHeight * scale;
			}

			Size getSize(const std::string_view text) const;
			Size getSize(const std::string_view text, uint8_t scale) const;

		private:
			const uint32_t _fromCodepoint;
			const uint32_t _toCodepoint;
			const uint8_t _constantGlyphWidth;
			const uint8_t _lineHeight;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}