#pragma once

#include "cstdlib"

namespace yoba {
	enum class ColorType : uint8_t {
		Palette,
		HSB,
		RGB888
	};

	class Rgb888Color;

	class Color {
		public:
			explicit Color(ColorType type);

			ColorType getType() const;

		private:
			ColorType _type;
	};

	class HsbColor : Color {
		public:
			HsbColor(float h, float s, float b);

			Rgb888Color toRgb888() const;

			float _h = 0;
			float _s = 0;
			float _b = 0;
	};

	class Rgb888Color : public Color {
		public:
			Rgb888Color();

			Rgb888Color(uint8_t r, uint8_t g, uint8_t b);
			Rgb888Color(const Rgb888Color &source);
			explicit Rgb888Color(uint32_t rgb888);
			explicit Rgb888Color(uint16_t rgb565);

			uint8_t getR() const;
			void setR(uint8_t r);

			uint8_t getG() const;
			void setG(uint8_t g);

			uint8_t getB() const;
			void setB(uint8_t b);

			uint32_t toUint32() const;
			uint16_t toUint16() const;

			void interpolateTo(Rgb888Color& second, float position);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	class PaletteColor : public Color {
		public:
			explicit PaletteColor(uint8_t index);

			uint8_t getIndex() const;
			void setIndex(uint8_t index);

		private:
			uint8_t _index;
	};
}