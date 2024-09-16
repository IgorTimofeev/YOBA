#pragma once

#include "cstdlib"

namespace yoba {
	enum class ColorType : uint8_t {
		Bit16,
		Bit24,
		Palette
	};

	class Color {
		public:
			explicit Color(ColorType type);

			ColorType getType() const;

		private:
			ColorType _type;
	};

	class ColorHSB {
		public:
			ColorHSB(float h, float s, float b);

			float _h = 0;
			float _s = 0;
			float _b = 0;

	};

	class Color16 : public Color {
		public:
			explicit Color16(uint16_t value);

			uint16_t getValue() const;
			void setValue(uint16_t value);

		private:
			uint16_t _value;
	};

	class Color24 : public Color {
		public:
			static const Color24 black;
			static const Color24 white;

			Color24(uint8_t r, uint8_t g, uint8_t b);

			Color24(Color24 const &source);

			explicit Color24(const ColorHSB &source);

			Color24();

			explicit Color24(uint32_t value);

			void multiply(float factor);

			void add(uint8_t r, uint8_t g, uint8_t b);

			void add(const Color24& color);

			uint32_t toUint32() const;
			uint16_t toUint16() const;

			static Color24 fromUint16(uint16_t color) {
				return {
					(uint8_t) ((((color >> 11) & 0x1F) * 255 + 15) / 31),
					(uint8_t) ((((color >> 5) & 0x3F) * 255 + 31) / 63),
					(uint8_t) ((((color) & 0x1F) * 255 + 15) / 31)
				};
			}

			static float govnoDelta(const Color24& color1, const Color24& color2) {
				float dr = (float) color1._r - (float) color2._r;
				float dg = (float) color1._g - (float) color2._g;
				float db = (float) color1._b - (float) color2._b;

				return (0.2126f * dr * dr + 0.7152f * dg * dg + 0.0722f * db * db);
			}

			static uint8_t interpolateChannel(uint8_t first, uint8_t second, float position);

			void interpolateTo(Color24& second, float position);

			uint8_t getR() const;
			void setR(uint8_t r);

			uint8_t getG() const;
			void setG(uint8_t g);

			uint8_t getB() const;
			void setB(uint8_t b);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	class ColorPalette : public Color {
		public:
			explicit ColorPalette(uint8_t index);

			uint8_t getIndex() const;
			void setIndex(uint8_t index);

			static uint8_t getClosestIndex(const uint16_t* palette, size_t _paletteLength, const Color24& color) {
				Color24 palette24;
				uint8_t closestIndex = 0;
				float delta;
				float closestDelta = 999999;

				for (size_t i = 0; i < _paletteLength; i++) {
					palette24 = Color24::fromUint16(palette[i]);

					delta = Color24::govnoDelta(palette24, color);

					if (delta < closestDelta) {
						closestDelta = delta;
						closestIndex = i;
					}
				}

				return closestIndex;
			}

		private:
			uint8_t _index;
	};
}