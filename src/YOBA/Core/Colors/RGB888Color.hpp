#pragma once

#include <functional>

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class MonochromeColor;
	class RGB565Color;
	class RGB666Color;
	class ARGBColor;
	class HSBColor;

	#pragma pack(push, 1)

	class RGB888Color : public Color {
		public:
			friend MonochromeColor;
			friend RGB565Color;
			friend RGB666Color;
			friend ARGBColor;
			friend HSBColor;

			constexpr RGB888Color(const uint8_t r, const uint8_t g, const uint8_t b) :
				Color(ColorModel::RGB888),
				_r(r),
				_g(g),
				_b(b)
			{

			}

			constexpr RGB888Color() : RGB888Color(0x00, 0x00, 0x00) {

			}

			constexpr RGB888Color(const uint32_t rgb888) : RGB888Color(
				(rgb888 >> 16) & 0xFF,
				(rgb888 >> 8) & 0xFF,
				rgb888 & 0xFF
			) {

			}

			constexpr uint8_t getR() const {
				return _r;
			}

			constexpr void setR(const uint8_t r) {
				_r = r;
			}

			constexpr uint8_t getG() const {
				return _g;
			}

			constexpr void setG(const uint8_t g) {
				_g = g;
			}

			constexpr uint8_t getB() const {
				return _b;
			}

			constexpr void setB(const uint8_t b) {
				_b = b;
			}

			constexpr uint32_t toUint32() const {
				return (_r << 16) | (_g << 8) | _b;
			}

			constexpr void interpolate(const RGB888Color& second, const float position);
			constexpr void blend(const ARGBColor& overlayColor);

			constexpr static void blend(
				uint8_t& r1,
				uint8_t& g1,
				uint8_t& b1,

				const uint8_t a2,
				const uint8_t r2,
				const uint8_t g2,
				const uint8_t b2
			) {
				// If overlay color is completely transparent, keeping current color as is
				if (a2 == 0x00) {
					return;
				}
				// If overlay color is completely NOT transparent, using overlay color
				else if (a2 == 0xFF) {
					r1 = r2;
					r1 = g2;
					r1 = b2;
				}

				const uint16_t invertedAlpha = 0xFF - a2;

				// "+ 128" means rounding to the closest channel value
				// ">> 8" means fast division by 0xFF
				r1 = static_cast<uint8_t>((r1 * invertedAlpha + r2 * a2 + 128) >> 8);
				g1 = static_cast<uint8_t>((g1 * invertedAlpha + g2 * a2 + 128) >> 8);
				b1 = static_cast<uint8_t>((b1 * invertedAlpha + b2 * a2 + 128) >> 8);
			}

			// RRRRRGGG GGGBBBBB
			constexpr MonochromeColor toMonochrome() const;
			constexpr RGB565Color toRGB565() const;
			constexpr RGB666Color toRGB666() const;
			constexpr ARGBColor toARGB() const;

			constexpr static void toHSB(
				const uint8_t red,
				const uint8_t green,
				const uint8_t blue,

				float& hue,
				float& saturation,
				float& brightness
			) {
				const auto redF = static_cast<float>(red) / 255.0f;
				const auto greenF = static_cast<float>(green) / 255.0f;
				const auto blueF = static_cast<float>(blue) / 255.0f;

				hue = 0;

				// Max of r/g/b
				brightness =
					redF > greenF
					? redF > blueF ? redF : blueF
					: greenF > blueF ? greenF : blueF;

				// Max - min of r/g/b
				const auto delta =
					brightness - (
						redF < greenF
						? redF < blueF ? redF : blueF
						: greenF < blueF ? greenF : blueF
					);

				saturation = brightness == 0 ? 0 : delta / brightness;

				if (saturation == 0)
					return;

				// Determining hue sector
				if (redF == brightness) {
					hue = (greenF - blueF) / delta;
				}
				else if (greenF == brightness) {
					hue = 2 + (blueF - redF) / delta;
				}
				else if (blueF == brightness) {
					hue = 4 + (redF - greenF) / delta;
				}

				// Sector to hue
				hue *= 1.0f / 6.0f;

				// For cases like R = MAX & B > G
				if (hue < 0)
					hue += 1.0f;
			}

			constexpr HSBColor toHSB() const;

			constexpr void toModel(const ColorModel targetModel, const std::function<void(const Color* color)>& convertedColorHandler) const;

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	using RGBColor = RGB888Color;

	#pragma pack(pop)
}