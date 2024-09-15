#pragma once

namespace yoba {
	class Glyph {
		public:
			Glyph(
				const uint32_t bitmapBitIndex,
				const uint16_t width
			) :
				_bitmapBitIndex(bitmapBitIndex),
				_width(width)
			{

			}

			uint32_t getBitmapBitIndex() const {
				return _bitmapBitIndex;
			}

			uint8_t getWidth() const {
				return _width;
			}

		private:
			const uint32_t _bitmapBitIndex = 0;
			const uint8_t _width;
	};

	class Font {
		public:
			Font(
				const uint16_t fromCodepoint,
				const uint16_t toCodepoint,
				const uint8_t height,
				const Glyph* glyphs,
				const uint8_t* bitmap
			) :
				_fromCodepoint(fromCodepoint),
				_toCodepoint(toCodepoint),
				_height(height),
				_glyphs(glyphs),
				_bitmap(bitmap)
			{

			}

			uint16_t getFromCodepoint() const {
				return _fromCodepoint;
			}

			uint16_t getToCodepoint() const {
				return _toCodepoint;
			}

			const Glyph* getGlyphs() const {
				return _glyphs;
			}

			const uint8_t* getBitmap() const {
				return _bitmap;
			}

			const Glyph* getGlyph(uint32_t codepoint) const {
				return
					codepoint < _fromCodepoint || codepoint > _toCodepoint
					? nullptr :
					&_glyphs[codepoint - _fromCodepoint];
			}

			uint16_t getWidth(const char *text) const {
				const char* charPtr;
				size_t charIndex = 0;
				const Glyph* glyph;

				uint16_t width = 0;

				while (true) {
					charPtr = text + charIndex;

					// End of text
					if (*charPtr == '\0')
						break;

					// Trying to find glyph matched to char
					glyph = getGlyph(*charPtr);

					if (glyph) {
						width += glyph->getWidth();
					}
						// For non-existing glyphs we can just simulate whitespace
					else {
						width += 10;
					}

					charIndex++;
				}

				return width;
			}

			uint16_t getWidth(const String& text) const {
				return getWidth(text.c_str());
			}

			uint8_t getHeight() const {
				return _height;
			}

			Size getSize(const char *text) const {
				return {
					getWidth(text),
					getHeight()
				};
			}

			Size getSize(const String& text) const {
				return getSize(text.c_str());
			}

		private:
			const uint16_t _fromCodepoint;
			const uint16_t _toCodepoint;
			const uint8_t _height;
			const Glyph* _glyphs;
			const uint8_t* _bitmap;
	};
}