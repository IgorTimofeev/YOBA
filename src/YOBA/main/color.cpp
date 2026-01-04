#include "color.h"

#include "math.h"

namespace YOBA {
	ColorModel Color::getModel() const {
		return _model;
	}

	// -------------------------------- Color --------------------------------

	uint8_t Color::getBytesPerModel(const ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::RGB565:
				return 2;

			case ColorModel::RGB666:
			case ColorModel::RGB888:
				return 3;

			default:
				return 1;
		}
	}

	size_t Color::getBytesPerModel(const size_t pixelCount, const ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::monochrome:
				return pixelCount >= 8 ? pixelCount / 8 : 1;

			case ColorModel::RGB565:
				return pixelCount * 2;

			case ColorModel::RGB666:
			case ColorModel::RGB888:
				return pixelCount * 3;

			default:
				return pixelCount;
		}
	}

	// -------------------------------- Rgb888Color --------------------------------

	uint32_t RGB888Color::toUint32() const {
		return r << 16 | g << 8 | b;
	}

	RGB565Color RGB888Color::toRGB565() const {
		return RGB565Color(r >> 3 << 3 | g >> 5 | g >> 2 << 13 | b >> 3 << 8);
//		return Rgb565Color(((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3));
	}

	RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color(r << 16 | g << 8 | b);
	}

	MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor(r > 0 || g > 0 || b > 0);
	}

	void RGB888Color::interpolateTo(const RGB888Color &second, const float position) {
		r = static_cast<uint8_t>(interpolate(static_cast<float>(r), static_cast<float>(second.r), position));
		g = static_cast<uint8_t>(interpolate(static_cast<float>(g), static_cast<float>(second.g), position));
		b = static_cast<uint8_t>(interpolate(static_cast<float>(b), static_cast<float>(second.b), position));
	}

	uint8_t RGB888Color::getR() const {
		return r;
	}

	void RGB888Color::setR(const uint8_t r) {
		this->r = r;
	}

	uint8_t RGB888Color::getG() const {
		return g;
	}

	void RGB888Color::setG(const uint8_t g) {
		this->g = g;
	}

	uint8_t RGB888Color::getB() const {
		return b;
	}

	void RGB888Color::setB(const uint8_t b) {
		this->b = b;
	}

	// -------------------------------- HsbColor --------------------------------
	
	float HSBColor::getH() const {
		return h;
	}

	void HSBColor::setH(const float h) {
		this->h = h;
	}

	float HSBColor::getS() const {
		return s;
	}

	void HSBColor::setS(const float s) {
		this->s = s;
	}

	float HSBColor::getB() const {
		return b;
	}

	void HSBColor::setB(const float b) {
		this->b = b;
	}

	RGB888Color HSBColor::toRgb888() const {
		RGB888Color result;

		const auto hueSector = h * 6.0f;
		const auto hueSectorIntegerPart = static_cast<uint8_t>(hueSector);
		const auto hueSectorFractionalPart = hueSector - static_cast<float>(hueSectorIntegerPart);

		const auto p = static_cast<uint8_t>(255.0f * b * (1 - s));
		const auto q = static_cast<uint8_t>(255.0f * b * (1 - hueSectorFractionalPart * s));
		const auto t = static_cast<uint8_t>(255.0f * b * (1 - (1 - hueSectorFractionalPart) * s));
		const auto v = static_cast<uint8_t>(255.0f * b);

		switch (hueSectorIntegerPart) {
			case 1:
				result.setR(q);
				result.setG(v);
				result.setB(p);
				break;

			case 2:
				result.setR(p);
				result.setG(v);
				result.setB(t);
				break;

			case 3:
				result.setR(p);
				result.setG(q);
				result.setB(v);
				break;

			case 4:
				result.setR(t);
				result.setG(p);
				result.setB(v);
				break;

			case 5:
				result.setR(v);
				result.setG(p);
				result.setB(q);
				break;

			default:
				result.setR(v);
				result.setG(t);
				result.setB(p);
				break;
		}

		return result;
	}

	// -------------------------------- MonochromeColor --------------------------------

	RGB888Color MonochromeColor::toRgb888() const {
		return
			_value > 0
			? RGB888Color(0xFf, 0xFF, 0xFF)
			: RGB888Color(0x00, 0x00, 0x00);
	}

	// -------------------------------- Rgb565Color --------------------------------

	RGB888Color RGB565Color::toRgb888() const {
		return {
			static_cast<uint8_t>(((_value >> 11 & 0x1F) * 255 + 15) / 31),
			static_cast<uint8_t>(((_value >> 5 & 0x3F) * 255 + 31) / 63),
			static_cast<uint8_t>(((_value & 0x1F) * 255 + 15) / 31)
		};
	}

	// -------------------------------- Rgb666Color --------------------------------

	RGB888Color RGB666Color::toRgb888() const {
		return {
			static_cast<uint8_t>((_value >> 12 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value >> 6 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value & 0b111111) * 255 / 0b111111)
		};
	}

	// -------------------------------- PaletteColor --------------------------------
	
}