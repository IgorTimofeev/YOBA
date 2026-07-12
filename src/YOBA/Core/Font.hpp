#pragma once

#include <cstdlib>
#include <string>

#include <YOBA/Core/Glyph.hpp>
#include <YOBA/Core/Size.hpp>

namespace YOBA {
	class Font {
		public:
			constexpr Font(
				const uint8_t constantGlyphWidth,
				const uint8_t lineHeight,
				const Glyph* glyphs,
				const uint8_t* bitmap
			) :
				_constantGlyphWidth(constantGlyphWidth),
				_lineHeight(lineHeight),
				_glyphs(glyphs),
				_bitmap(bitmap)
			{

			}

			constexpr virtual ~Font() = default;

			constexpr static uint8_t missingGlyphWidth = 10;

			/**
			 * Accepts a UTF-8 codepoint and converts it to a glyph index in the corresponding array.
			 *
			 * This method must be implemented in the font subclasses.
			 *
			 * @param codepoint UTF-8 codepoint
			 * @return Positive index of glyph if it was found in map and -1 otherwise
			 */
			constexpr virtual int32_t getGlyphIndex(const uint32_t codepoint) const = 0;

			constexpr bool isConstantGlyphWidth() const {
				return _constantGlyphWidth > 0;
			}

			constexpr uint8_t getConstantGlyphWidth() const {
				return _constantGlyphWidth;
			}

			constexpr const uint8_t* getBitmap() const {
				return _bitmap;
			}

			/// @param glyphIndex Index of glyph obtained via getGlyphIndex(codepoint)
			/// @return Pointer to glyph that will never be nullptr.
			constexpr const Glyph* getGlyphByIndex(const uint32_t glyphIndex) const {
				return
					isConstantGlyphWidth()
					? _glyphs + glyphIndex
					: reinterpret_cast<const VariableWidthGlyph*>(_glyphs) + glyphIndex;
			}

			/// Returns bitmap bit index of glyph
			/// @param glyphIndex Glyph index in glyphs array
			/// @param glyph Glyph itself
			/// @return
			constexpr uint32_t getBitmapBitIndex(const uint32_t glyphIndex, const Glyph* glyph) const {
				return
					isConstantGlyphWidth()
					? glyphIndex * (getConstantGlyphWidth() * getLineHeight())
					: reinterpret_cast<const VariableWidthGlyph*>(glyph)->getBitmapBitIndex();
			}

			/// @param glyph Pointer to glyph whose width is to be returned. Must not be nullptr.
			/// @return Width of glyph in pixels.
			constexpr uint8_t getWidth(const Glyph* glyph) const {
				return
					isConstantGlyphWidth()
					? _constantGlyphWidth
					: reinterpret_cast<const VariableWidthGlyph*>(glyph)->getWidth();
			}

			constexpr uint8_t getWidth(const Glyph* glyph, const uint8_t scale) const {
				return getWidth(glyph) * scale;
			}

			constexpr uint8_t getWidth(const uint32_t codepoint) const {
				const auto glyphIndex = getGlyphIndex(codepoint);

				return
					glyphIndex >= 0
					? getWidth(getGlyphByIndex(glyphIndex))
					: missingGlyphWidth;
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
			const uint8_t _constantGlyphWidth;
			const uint8_t _lineHeight;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}