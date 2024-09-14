#pragma once

#include <cstdint>

namespace yoba {
	enum class ColorType : uint8_t {
		Bit8,
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
			static const Color16 black;
			static const Color16 white;

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

			uint32_t to24Bit() const;
			uint16_t to16Bit() const;

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

		private:
			uint8_t _index;
	};
}